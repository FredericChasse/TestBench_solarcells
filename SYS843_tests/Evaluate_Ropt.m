clear all
close all
clc

% S0 = 600 : Ropt = 67.1 + beta
% S0 = 300 : Ropt = 156  + beta

S0Vec = [200];
gammaVec = [0];
betaVec = [0];

oDoBetaDif = 0;
oDoGammaDif = 0;

RextOpt = zeros(length(S0Vec) * length(gammaVec) * length(betaVec), 1);

iRext = 0;

for j_Gamma = 1 : length(gammaVec)
  for k_Beta = 1 : length(betaVec)
    for i_S0 = 1 : length(S0Vec)  
      
      iRext = iRext + 1;
      
      if betaVec(k_Beta) ~= 0
        oDoBetaDif = 1;
      else
        oDoBetaDif = 0;
      end
      
      if gammaVec(j_Gamma) ~= 0
        oDoGammaDif = 1;
      else
        oDoGammaDif = 0;
      end
      
      S0 = S0Vec(i_S0);
      gamma = gammaVec(j_Gamma);
      beta  = betaVec(k_Beta);
      
      mfcRun
      
      RextOpt(iRext) = Ropt;
    end
  end
end
  