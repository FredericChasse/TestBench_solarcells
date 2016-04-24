% clear all
% close all
% clc

oDoFigures = 1;

% nIterations = 100;
% iPerturb = 60;
% 
% oDoPerturbStatic  = 0;
% oDoPerturbDynamic = 0;
% oDoGammaDif       = 0;
% oDoGammaBetaDif   = 0;
% oDoBetaDif        = 0;
% gamma = 0.0001;
% beta = 10;

% S0 = ones(2, nIterations) * 450;
S0 = ones(2, nIterations) * 300;
if oDoPerturbStatic
  S0(:,iPerturb:end) = S0(1,1) + perturbAmp;
end
if oDoPerturbDynamic
  t = iPerturb:1:nIterations;
  x = sin(t/2)*25 + 650;
  for iUnit = 1:2
    S0(iUnit,iPerturb:end) = x;
  end
  clear t x
end
if oDoGammaBetaDif
  S0(1, :) = 500;
end

T = 1;

mfcDynamics = zeros(2, 4);
mfcDynamics(1, :) = [20.8395  498.2432    2.0000    0.0412];
mfcDynamics(2, :) = [20.8395  498.2432    2.0000    0.0412];

odeOptions = odeset('RelTol',1e-6,'AbsTol',1e-9);

Pout = zeros(2, nIterations);

alpha = 20000;
delta = 20;

RextMax = 1050;
RextMin = 50;

Rres = 0.1;

Rinit = 200;

Rext = zeros(nIterations, 2);

Rext(1, 1) = Rinit - delta/2;
Rext(1, 2) = Rinit + delta/2;
% Rext(1) = rand*(RextMax - RextMin) + RextMin;

cmd = [0 Rinit];
grad = [0 0];
format short

gradMem = zeros(nIterations, 2);
cmdMem  = zeros(nIterations, 2);
ni = 1:nIterations;
ni = ni';

tic
waitBarHandler = waitbar(0);
for i = 1 : nIterations
  
  waitbar(i/nIterations);
  
  % Unit 1
  if oDoBetaDif
    [tt, Y] = ode15s('mfcModel', [0 T], mfcDynamics(1, :), odeOptions, S0(1, i), Rext(i, 1) + beta);
    mfcDynamics(1, :) = Y(end, :);
    [dummy, Pout(1, i)] = mfcModel(T, mfcDynamics(1, :), odeOptions, S0(1, i), Rext(i, 1) + beta);
  else
    [tt, Y] = ode15s('mfcModel', [0 T], mfcDynamics(1, :), odeOptions, S0(1, i), Rext(i, 1));
    mfcDynamics(1, :) = Y(end, :);
    [dummy, Pout(1, i)] = mfcModel(T, mfcDynamics(1, :), odeOptions, S0(1, i), Rext(i, 1));
  end
  
  if oDoGammaDif
    Pout(1, i) = Pout(1, i) + gamma;
  end
  
  % Unit 2
  [tt, Y] = ode15s('mfcModel', [0 T], mfcDynamics(2, :), odeOptions, S0(2, i), Rext(i, 2));
  mfcDynamics(2, :) = Y(end, :);
  [dummy, Pout(2, i)] = mfcModel(T, mfcDynamics(2, :), odeOptions, S0(2, i), Rext(i, 2));
  
  % Tustin's discrete integrator
  grad(1) = grad(2);
  grad(2) = alpha * (Pout(2, i) - Pout(1, i));
  cmd(1) = cmd(2);
  cmd(2) = cmd(1) + T/2*(grad(1) + grad(2));
  
  if abs(cmd(2) - cmd(1)) < Rres
    cmd(2) = cmd(1);
  end
  
  Rext(i+1, 1) = cmd(2) - delta/2;
  Rext(i+1, 2) = cmd(2) + delta/2;
  
  if Rext(i+1, 2) > RextMax
    Rext(i+1, 1) = RextMax - delta;
    Rext(i+1, 2) = RextMax;
  end
  if Rext(i+1, 1) < RextMin
    Rext(i+1, 1) = RextMin;
    Rext(i+1, 2) = RextMin + delta;
  end
  
%   if (i < nIterations)
%     Rext(i+1) = min(RextMax, cmd(2));
%     Rext(i+1) = max(RextMin, Rext(i+1));
%   end
  gradMem(i, :) = grad;
  cmdMem (i, :) = cmd;
end
toc

if oDoFigures
  subplot(2,1,1)
  plot(Pout(1,:))
  hold on
  plot(Pout(2,:))
  legend({'1' '2'})
  hold off
  
  subplot(2,1,2)
  plot(Rext(:,1))
  hold on
  plot(Rext(:,2))
  legend({'1' '2'})
  hold off
end

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
  iSteadyState = iSteadyState - iStart;
else
  iSteadyState;
end