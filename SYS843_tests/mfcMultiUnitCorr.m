clear all
close all
% clc

%% Simulation setup

nIterations = 200;
iPerturb = 60;

% Test cases
oDoPerturbStatic  = 0;
oDoPerturbDynamic = 0;
oDoGammaDif       = 1;
oDoBetaDif        = 0;
gamma             = 0.0001;
beta              = 30;

S0 = ones(2, nIterations) * 300;
if oDoPerturbStatic
  S0(:,iPerturb:end) = 600;
end
if oDoPerturbDynamic
  t = iPerturb:1:nIterations;
  x = sin(t/2)*25 + 650;
  for iUnit = 1:2
    S0(iUnit,iPerturb:end) = x;
  end
  clear t x
end

% Model parameters
T_model = 1;

mfcDynamics = zeros(2, 4);
mfcDynamics(1, :) = [20.8395  498.2432    2.0000    0.0412];
mfcDynamics(2, :) = [20.8395  498.2432    2.0000    0.0412];

odeOptions = odeset('RelTol',1e-6,'AbsTol',1e-9);

delta = 20;
k_mu = 80000*delta;

RextMax = 1050;
RextMin = 20;

Rext = zeros(nIterations, 2);

Rinit = 400;
Rext(1,1) = Rinit - delta/2;
Rext(1,2) = Rinit + delta/2;

u = [0 Rinit];
grad = [0 0];

% Corrector
kBeta = 100000;
kGamma = 1;
aCorr = 10;

betaCorr = [0 0];
betaCorr_p = [0 0];
gammaCorr = [0 0];
gammaCorr_p = [0 0];
u_p = [0 0];

T = 30;
T1 = 22;
T2 = 8;

dMu = zeros(nIterations, 1);
dCorr = zeros(nIterations, 1);

for i = 0 : T : nIterations - T
  dMu(i+1 : i + T1) = 1;
  dMu(i + T1 + 1 : i + T) = 0;
  
  dCorr(i+1 : i + T1) = 0;
  
  for j = i+T1+1 : 2 : i + T1 + T2
    dCorr(j) = -1;
    dCorr(j+1) = 1;
  end
end

gradMem       = zeros(nIterations, 2);
uMem          = zeros(nIterations, 2);
betaCorrMem   = zeros(nIterations, 2);
gammaCorrMem  = zeros(nIterations, 2);
upMem         = zeros(nIterations, 2);

Pout          = zeros(2, nIterations);

ni = 1:nIterations;
ni = ni';

format short

%% Simulation

tic
waitBarHandler = waitbar(0);
for i = 1 : nIterations
  waitbar(i/nIterations);
    
  % Unit 1
  [tt, Y] = ode15s('mfcModel', [0 T_model], mfcDynamics(1, :), odeOptions, S0(1, i), Rext(i, 1));
  mfcDynamics(1, :) = Y(end, :);
  [dummy, Pout(1, i)] = mfcModel(4, mfcDynamics(1, :), odeOptions, S0(1, i), Rext(i, 1));
  if oDoGammaDif
    Pout(1, i) = Pout(1, i) + gamma;
  end
  
  % Unit 2
  if oDoBetaDif
    [tt, Y] = ode15s('mfcModel', [0 T_model], mfcDynamics(2, :), odeOptions, S0(2, i), Rext(i, 2) + beta);
    mfcDynamics(2, :) = Y(end, :);
    [dummy, Pout(2, i)] = mfcModel(4, mfcDynamics(2, :), odeOptions, S0(2, i), Rext(i, 2) + beta);
  else
    [tt, Y] = ode15s('mfcModel', [0 T_model], mfcDynamics(2, :), odeOptions, S0(2, i), Rext(i, 2));
    mfcDynamics(2, :) = Y(end, :);
    [dummy, Pout(2, i)] = mfcModel(4, mfcDynamics(2, :), odeOptions, S0(2, i), Rext(i, 2));
  end
  
  % Tustin's discrete integrator
%   grad(1) = grad(2);
%   grad(2) = k_mu/delta * (Pout(2, i) - Pout(1, i));
%   u(1) = u(2);
%   u(2) = u(1) + T_model/2*(grad(1) + grad(2));
%   
%   Rext(i+1, 1) = u(2) - delta/2;
%   Rext(i+1, 2) = u(2) + delta/2;
  
  u_p(1) = u_p(2);
  u_p(2) = k_mu/delta * (Pout(2, i) - Pout(1, i) - gammaCorr(2))*dMu(i);
  u(1) = u(2);
  u(2) = u(1) + T_model/2*(u_p(1) + u_p(2));
  
  betaCorr_p(1) = betaCorr_p(2);
  betaCorr_p(2) = kBeta/aCorr * (Pout(2, i) - Pout(1, i) - gammaCorr(2))*dCorr(i);
  betaCorr(1) = betaCorr(2);
  betaCorr(2) = betaCorr(1) + T_model/2*(betaCorr_p(1) + betaCorr_p(2));
  
  gammaCorr_p(1) = gammaCorr_p(2);
  gammaCorr_p(2) = kGamma*(Pout(2, i) - Pout(1, i) - gammaCorr(2))*(1-dMu(i));
  gammaCorr(1) = gammaCorr(2);
  gammaCorr(2) = gammaCorr(1) + T_model/2*(gammaCorr_p(1) + gammaCorr_p(2));
  
  Rext(i+1, 1) = u(2) - dMu(i)*delta/2 + aCorr*dCorr(i);
  Rext(i+1, 2) = u(2) + dMu(i)*delta/2 + aCorr*dCorr(i) - betaCorr(2);
  
  if Rext(i+1, 2) > RextMax
    Rext(i+1, 1) = RextMax - delta;
    Rext(i+1, 2) = RextMax;
  end
  if Rext(i+1, 1) < RextMin
    Rext(i+1, 1) = RextMin;
    Rext(i+1, 2) = RextMin + delta;
  end
  
  uMem        (i, :) = u;
  betaCorrMem (i, :) = betaCorr;
  gammaCorrMem(i, :) = gammaCorr;
  gradMem     (i, :) = grad;
  upMem       (i, :) = u_p;
  uMem        (i, :) = u;
end
toc

%% Figures

subplot(2,1,1)
plot(Pout(1,:))
hold on
plot(Pout(2,:))
legend({'1' '2'})
subplot(2,1,2)
plot(Rext(:, 1))
hold on
plot(Rext(:, 2))
legend({'1' '2'})

close(waitBarHandler)

%% Steady-state

ssPrecision = 0.05;

iSteadyState = nIterations + 1;

if oDoPerturbStatic || oDoPerturbDynamic
  iStart = iPerturb;
else
  iStart = 1;
end

for iData = iStart : nIterations

  meanD = mean(Rext(iData:end, :));
    
  for jData = iData : nIterations
    if ( mean( (Rext(jData, :) <= (1 + ssPrecision).*meanD) & (Rext(jData, :) >= (1 - ssPrecision).*meanD) ) ~= 1 )
      iSteadyState = nIterations + 1;
      break;
    else
      iSteadyState = iData;
    end
  end
  
  if (iSteadyState ~= nIterations + 1)
    break;
  end
end

if iStart ~= 1
  iSteadyState = iSteadyState - iStart
else
  iSteadyState
end