clear all
close all

gammaVec = [0 0.0001 0.0005];
% gammaVec = [0.0002 0.0004];
betaVec = [0 8 30];
% perturbVec = [0 150 -150];
perturbVec = [150];

% S0 = 300 : Ropt = 156.0 + beta
% S0 = 450 : Ropt =  96.5 + beta
% S0 = 600 : Ropt =  67.1 + beta

% Ropt = [156.016 67.071];

nParticles = 8;

% RoptBasic = [96.5 67.1 156];
RoptBasic = [96.5];
Ropt = zeros(1, nParticles);

oDoPerturbStatic  = 0;
oDoPerturbDynamic = 0;
oDoGammaDif       = 0;
oDoBetaDif        = 0;
oDoGammaBetaDif   = 0;

% nIterations = 80;
% iPerturb = 40;
nIterations = 200;
iPerturb = 100;

perturbAmp = 150;

iTestCase = 0;

nTestForMean = 20;

nTestCases = length(gammaVec) * length(betaVec) * length(perturbVec);

convergence = zeros(nTestForMean, nTestCases);
precision   = zeros(nTestForMean, nParticles, nTestCases);

convergenceMean = zeros(nTestCases, 1);
precisionMean   = zeros(nTestCases, 1);

for i_Gamma = 1 : length(gammaVec)
  for j_Beta = 1 : length(betaVec)
    for k_Perturb = 1 : length(perturbVec)

      iTestCase = iTestCase + 1
      testCaseStr{iTestCase} = ['Beta = ' num2str(betaVec(j_Beta)) '; Gamma = ' num2str(gammaVec(i_Gamma)) '; Perturb = ' num2str(perturbVec(k_Perturb))];
      
      testCaseStr{iTestCase}
      
      for nTest = 1 : nTestForMean

        if betaVec(j_Beta) ~= 0
          oDoBetaDif = 1;
        else
          oDoBetaDif = 0;
        end

        if gammaVec(i_Gamma) ~= 0
          oDoGammaDif = 1;
        else
          oDoGammaDif = 0;
        end

        if perturbVec(k_Perturb) ~= 0
          oDoPerturbStatic = 1;
        else
          oDoPerturbStatic = 0;
        end

        perturbAmp = perturbVec(k_Perturb);
        gamma = gammaVec(i_Gamma);
        beta  = betaVec(j_Beta);

        PSO
        
        Ropt(1:4) = RoptBasic(k_Perturb);
        Ropt(5:8) = RoptBasic(k_Perturb) - beta;
        
        precision  (nTest, :, iTestCase) = (1 - abs(d(end, :) - Ropt) ./ Ropt) * 100;
        convergence(nTest,    iTestCase) = iSteadyState;
        for iParticle = 1 : nParticles
          if precision(nTest, iParticle, iTestCase) < 0
            precision(nTest, iParticle, iTestCase) = 0;
          end
        end
        precision  (nTest, :, iTestCase)
        convergence(nTest,    iTestCase)
      end
      
    end
  end
end

for iTestCase = 1 : nTestCases
  precisionMean  (iTestCase) = mean(mean(precision(:,:, iTestCase)));
  convergenceMean(iTestCase) = mean(convergence(:, iTestCase));
end

% convergenceMean = iSteadyState;
% precisionMean(:) = mean(precision(:,:,:));
% for i = 1 : 8
%   precision = precision + d(end, i)/Ropt(i);
% end
% precision = precision/8 * 100;