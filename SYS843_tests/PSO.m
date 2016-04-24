%% clear variables and close figures
% clear all

% if ( ~exist('rngState', 'var') )
%   rngState = rng;
% end
% clearvars -except rngState
% rng(rngState);

% close all
% clc

oFigures = 1;   %% 0 = don't print figures

%% Simulation parameters

% nIterations = 200;
% nParticles  = 8;

omega = 0.5;

% For test purposes
ni = zeros(nIterations, 1);
for iData = 1 : nIterations
  ni(iData) = iData;
end


% Search space
dmin        = 50;
dmax        = 1050;


% RextResolution = (dmax-dmin)/255; % Ohms
RextResolution = 0.1; % Ohms
Rvalues = dmin:RextResolution:dmax;

% Steady-state precision
ssPrecision = 0.05;   % 5% precision for calculating steady-state


% Environment change
detectPrecision = 0.01;   % 5% precision
oChangeHasOccured = 0;    % Flag for detecting change


% Simulation data
d           = zeros(nIterations, nParticles);
J           = zeros(nIterations, nParticles);
MaxVal      = zeros(nIterations, nParticles);
Pbest       = zeros(nIterations, nParticles);
v           = zeros(nIterations, nParticles);

MaxOfUnits  = zeros(nIterations, 1);
Gbest       = zeros(nIterations, 1);
c           = zeros(nIterations, 2);

% Initial position of particles
for iUnit = 1 : nParticles
  d(1, iUnit) = rand* (dmax-dmin) + dmin;
  tmp = abs(Rvalues - d(1, iUnit));
  [idx idx] = min(tmp);
  d(1, iUnit) = Rvalues(idx);
end

%% Dynamic model parameters

mfcDynamics = zeros(nParticles, 4);

for i = 1:nParticles
  mfcDynamics(i,:) = [20.8395  498.2432    2.0000    0.0412];
end

% oDoPerturbStatic  = 0;
% oDoPerturbDynamic = 0;
% oDoGammaDif       = 0;
% oDoBetaDif        = 1;
% oDoGammaBetaDif   = 0;
% gamma             = 0.0002;
% beta              = 20;
% 
% iPerturb = 30;
% perturbAmp = 300;

% S0 = ones(nParticles, nIterations) * 450;
S0 = ones(nParticles, nIterations) * 300;

if oDoGammaBetaDif
  S0(1:nParticles/2, :) = 350;
end
if oDoPerturbStatic
  for iUnit = 1 : nParticles
    S0(iUnit,iPerturb:end) = S0(iUnit,1) + perturbAmp;
  end
end
if oDoPerturbDynamic
  t = iPerturb:1:nIterations;
  x = sin(t/2)*5 + 450;
  for iUnit = 1:nParticles
    S0(iUnit,iPerturb:end) = x;
  end
  clear t x
end

odeOptions = odeset('RelTol',1e-6,'AbsTol',1e-9);
% odeOptions = odeset('RelTol',1e-9,'AbsTol',1e-12);

T = 2;

%% Simulation
tic
waitBarHandler = waitbar(0);
for iData = 1 : nIterations

  waitbar(iData/nIterations);
  
  % Curve (J)
  %========================================================================
  for iUnit = 1 : nParticles
    
    if (iUnit > 4) && (oDoBetaDif)
      [tt, Y] = ode15s('mfcModel', [0 T], mfcDynamics(iUnit, :), odeOptions, S0(iUnit, iData), d(iData, iUnit) + beta);
      mfcDynamics(iUnit,:) = Y(end, :);
      [dummy, J(iData, iUnit)] = mfcModel(T, mfcDynamics(iUnit, :), odeOptions, S0(iUnit, iData), d(iData, iUnit) + beta);
    else
      [tt, Y] = ode15s('mfcModel', [0 T], mfcDynamics(iUnit, :), odeOptions, S0(iUnit, iData), d(iData, iUnit));
      mfcDynamics(iUnit,:) = Y(end, :);
      [dummy, J(iData, iUnit)] = mfcModel(T, mfcDynamics(iUnit,:), odeOptions, S0(iUnit, iData), d(iData, iUnit));
    end
    
    if (iUnit > 4) && (oDoGammaDif)
      J(iData, iUnit) = J(iData, iUnit) + gamma;
    end
  end
  %========================================================================

  % Global max values
  %========================================================================
  MaxOfUnits(iData) = max( J(iData, :) );
  %========================================================================

  % Local max values
  %========================================================================
  if (iData == 1)
    for iUnit = 1 : nParticles
      MaxVal(1, iUnit) = J(1, iUnit);
    end
  else
    for iUnit = 1 : nParticles
      MaxVal(iData, iUnit) = max( J(iData - 1 : iData, iUnit) );
%       MaxVal(iData, iUnit) = max( J(iData, iUnit), MaxVal(iData - 1, iUnit) );
    end
  end
  %========================================================================

  % Particle best (Pbest)
  %========================================================================
  if (iData == 1)
    for iUnit = 1 : nParticles
      Pbest(1, iUnit) = d(1, iUnit);
    end
  else
    for iUnit = 1 : nParticles
      if ( MaxVal(iData, iUnit) == J(iData, iUnit) )
        Pbest(iData, iUnit) = d(iData, iUnit);
      else
        Pbest(iData, iUnit) = Pbest(iData - 1, iUnit);
      end
    end
  end
  %========================================================================

  % Global best (Gbest)
  %========================================================================
  for iUnit = 1 : nParticles
%     if ( iData == 1)
      if ( MaxOfUnits(iData) == J(iData, iUnit) )
        Gbest(iData) = d(iData, iUnit);
      end
%     else
%       if ( MaxOfUnits(iData) == J(iData, iUnit) )
%         Gbest(iData) = d(iData, iUnit);
%         if ( Gbest(iData) < Gbest(iData - 1) )
%           Gbest(iData) = Gbest(iData - 1);
%         end
%       end
%     end
  end
  %========================================================================

  % Acceleration coefficients (c)
  %========================================================================
  if (iData == 1)
      c(1,1) = 2;   % Cognitive
      c(1,2) = 1;     % Social
  else
      c(iData,1) = c(iData - 1,1) + 1/nIterations;
      c(iData,2) = c(iData - 1,2) - 1/nIterations;
  end
  %========================================================================
  
  % Environment change detection
  %========================================================================
  if (iData > 3)
    for iUnit = 1 : nParticles
      % If no change has occured on the particle's position
      if ( d(iData, iUnit) == d(iData - 1, iUnit) )
        % If the particle's objective function is not the same as before
        if ( abs(J(iData, iUnit) - J(iData - 1, iUnit)) / J(iData - 1, iUnit) >= detectPrecision )
          oChangeHasOccured = 1;
        else
          oChangeHasOccured = 0;
        end
      end
      if (oChangeHasOccured)
        break;
      end
    end
  end
  %========================================================================
  
  % Particles' velocities (v)
  %========================================================================
  if (iData == 1)
    for iUnit = 1 : nParticles
      v(iData, iUnit) = rand                                                           ...
                      + c(iData, 1) * rand * (Pbest(iData, iUnit)  - d(iData, iUnit))  ...
                      + c(iData, 2) * rand * (Gbest(iData)         - d(iData, iUnit));
    end
  else
    for iUnit = 1 : nParticles
      v(iData, iUnit) = round                                                         ...
                      ( omega*v(iData-1, iUnit)                                       ...
                      + c(iData, 1) * rand * (Pbest(iData, iUnit)  - d(iData, iUnit)) ...
                      + c(iData, 2) * rand * (Gbest(iData)         - d(iData, iUnit)) ...
                      , 4);
    end
  end
  %========================================================================

  % Particles' positions (d)
  %========================================================================
  if (iData ~= nIterations)
    if (~oChangeHasOccured)
      for iUnit = 1 : nParticles
        nextPos = d(iData, iUnit) + v(iData, iUnit);
        tmp = abs(Rvalues - nextPos);
        [idx idx] = min(tmp);
        nextPos = Rvalues(idx);
        if ( nextPos < dmin )
          d(iData + 1, iUnit) = dmin;
        elseif ( nextPos > dmax )
          d(iData + 1, iUnit) = dmax;
        else
          d(iData + 1, iUnit) = nextPos;
        end
      end
    else
      for iUnit = 1 : nParticles
%         nextPos = rand* (dmax-dmin) + dmin;
        nextPos = d(iData, iUnit) + (rand*100 -50);
        tmp = abs(Rvalues - nextPos);
        [idx idx] = min(tmp);
        d(iData + 1, iUnit) = Rvalues(idx);
        if ( nextPos < dmin )
          d(iData + 1, iUnit) = dmin;
        elseif ( nextPos > dmax )
          d(iData + 1, iUnit) = dmax;
        else
          d(iData + 1, iUnit) = nextPos;
        end
        
        MaxVal(iData, iUnit) = 0;
        Pbest(iData, iUnit) = d(iData + 1, iUnit);
        v(iData, iUnit) = 0;
      end
      MaxOfUnits(iData) = 0;
      Gbest(iData) = d(iData + 1, 1);
      c(iData, 1) = c(1, 1);
      c(iData, 2) = c(1, 2);
      oChangeHasOccured = 0;
    end
  end
  %========================================================================
  
end
toc
close(waitBarHandler)
    
%% Steady-state

iSteadyState = nIterations + 1;

if oDoPerturbStatic || oDoPerturbDynamic
  iStart = iPerturb;
else
  iStart = 1;
end

for iData = iStart : nIterations

  meanD = mean(d(iData:end,:));
    
  for jData = iData : nIterations
    if ( mean( (d(jData, :) <= (1 + ssPrecision).*meanD) & (d(jData, :) >= (1 - ssPrecision).*meanD) ) ~= 1 )
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

%% Figures

if (oFigures)
  for iUnit = 1 : nParticles
    n = num2str(iUnit);
    Jstr(iUnit) = {strcat('J',n)};
    dstr(iUnit) = {strcat('d',n)};
    istr(iUnit) = {['Particule ' num2str(n)]};
  end

  fig1 = figure(1);
  subplot(2,1,1)
  for iUnit = 1 : nParticles
    plot(J(:,iUnit))
    hold on
  end
  legend(Jstr)
  hold off
  
  subplot(2,1,2)
  for iUnit = 1 : nParticles
    plot(d(:,iUnit))
    hold on
  end
  legend(dstr)
  hold off
end


% %% Figures
% 
% if (oFigures)
%   for iUnit = 1 : nParticles
%     n = num2str(iUnit);
%     Jstr(iUnit) = {strcat('J',n)};
%     dstr(iUnit) = {strcat('d',n)};
%     istr(iUnit) = {['Particule ' num2str(n)]};
%   end
% 
%   fig1 = figure(1);
%   hold on
%   for iUnit = 1 : nParticles
%     plot(J(:,iUnit))
%   end
%   legend(Jstr)
% 
%   fig2 = figure(2);
%   hold on
%   for iUnit = 1 : nParticles
%     plot(d(:,iUnit))
%   end
%   legend(dstr)
%   pos2 = fig2.Position;
%   fig2.Position = [pos2(1) 100 pos2(3) pos2(4)];
% 
%   fig3 = figure(3);
%   hold on
%   for iUnit = 1 : nParticles
%     plot(d(:,iUnit), J(:,iUnit), '.')
%   end
%   legend(istr)
%   pos3 = fig3.Position;
%   fig3.Position = [1860 pos3(2) pos3(3) pos3(4)];;
% 
%   fig4 = figure(4);
%   hold on
%   for iUnit = 1 : nParticles
%     plot(d(:,iUnit), J(:,iUnit))
%   end
%   legend(istr)
%   pos4 = fig4.Position;
%   fig4.Position = [1860 100 pos4(3) pos4(4)];
% end

%% Tests after run
% format short g
% d(1,:)
% d(end,:)
% [ni d]
% [ni Gbest Pbest]
% d;
% J;
% vJ = [v J]
% Jopt