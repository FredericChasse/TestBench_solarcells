clear all
close all

%% Port setup

% Wait for the user to start the program
reply = 'N';
while reply == 'N'
  reply = upper(input('Start? Y/N [N]:','s'));
  if isempty(reply)
    reply = 'Y'
  end
end

% Create a serial port object.
port = instrfind('Type', 'serial', 'Port', 'COM5', 'Tag', '');

if isempty(port)
    port = serial('COM5');
else
    fclose(port);
    port = port(1)
end

global nSolarCells;

nSolarCells = 4;

%% Matlab Mode
global matlabMode;

matlabMode = 'p';   % PSO
% matlabMode = 'c';   % Caracterization
% matlabMode = 'm';   % Multi-Unit

oDoPerturb = 0;
oDoPerturb = 1;
iPerturb   = 100;

global psoData;
psoData = [];

%% Data setup

if matlabMode == 'p'
%   port.BytesAvailableFcnCount = 224; % 56 floats to receive = 224 bytes
  port.BytesAvailableFcnCount = 112; % 56 floats to receive = 224 bytes
%   port.BytesAvailableFcnCount = 144; % 56 floats to receive = 224 bytes
elseif matlabMode == 'c'
  port.BytesAvailableFcnCount = 144; % 36 floats to receive = 144 bytes
elseif matlabMode == 'm'
  port.BytesAvailableFcnCount = 140; % 35 floats to receive = 140 bytes
end
port.BytesAvailableFcnMode = 'byte';
port.BytesAvailableFcn = {@myCallback};

port.BaudRate = 115200;
port.DataBits = 8;
port.Parity = 'none';
port.StopBits = 1;
port.Terminator = '';

% Connect to instrument object, port.
fopen(port);

if matlabMode == 'p' && oDoPerturb
  fwrite(port, 'P')
elseif matlabMode == 'm' && oDoPerturb
  fwrite(port, 'M')
else
  fwrite(port, matlabMode)
end

%% Figures init

if matlabMode == 'p'
  nSolarCells = 3;
%   nSolarCells = 4;
  
% Create figure for data
  for i = 1 : nSolarCells*2
    fig(i) = figure(i);
    hold on
  end
  
elseif matlabMode == 'c'
  nSolarCells = 8;
  
% Create figure for data
  for i = 1 : nSolarCells
    fig(i) = figure(i);
    hold on
  end
  
elseif matlabMode == 'm'
  nSolarCells = 2;
  
% Create figure for data
  for i = 1 : nSolarCells*2
    fig(i) = figure(i);
    hold on
  end
  
end


if (i == 4)
  pos2 = fig(2).Position;
  fig(2).Position = [1860 pos2(2) pos2(3) pos2(4)];
  pos3 = fig(3).Position;
  fig(3).Position = [pos3(1) 100 pos3(3) pos3(4)];
  pos4 = fig(4).Position;
  fig(4).Position = [1860 100 pos4(3) pos4(4)];
elseif (i == 6)
  fig(1).Position = [0        838 840 630];
  fig(2).Position = [840      838 840 630];
  fig(3).Position = [840+840  838 840 630];
  fig(4).Position = [0        88  840 630];
  fig(5).Position = [840      88  840 630];
  fig(6).Position = [840+840  88  840 630];
elseif (i == 8)
  fig(1).Position = [0 1070 640 430];
  fig(2).Position = [640 1070 640 430];
  fig(3).Position = [1280 1070 640 430];
  fig(4).Position = [1920 1070 640 430];
  fig(5).Position = [0 521 640 430];
  fig(6).Position = [640 521 640 430];
  fig(7).Position = [1280 521 640 430];
  fig(8).Position = [1920 521 640 430];
end

% Wait for the user to stop the program
reply = 'N';
while reply == 'N'
  reply = input('Stop? Y/N [Y]:','s');
  if isempty(reply)
    reply = 'Y'
  end
end

fwrite(port, 's')

% Disconnect from instrument object, obj1.
fclose(port);

% Clean up all objects.
delete(port);

%% Figures

if matlabMode == 'c'
  % Extract data from figure
  for i = 1 : nSolarCells
    axesObjs = get(fig(i), 'Children');
    dataObjs = get(axesObjs, 'Children');
    xdata = get(dataObjs, 'XData');
    ydata = get(dataObjs, 'YData');
    ndata = length(xdata);

    x1 = [xdata{ndata}];
    y1 = [ydata{ndata}];
    for j = ndata - 1 : -1 : 1
      x1 = [x1 xdata{j}];
      y1 = [y1 ydata{j}];
    end

    R(i,:) = x1./255.*1000 + 50;
  %   P(i,:) = y1.^2 ./ R(i,:);
    P(i,:) = y1;
  end

  fig(i+1) = figure(i +1);
  hold on
  set(gcf,'units','normalized','outerposition',[0 0 1 1])
  
  plotLines  = {'-' '-' '-' '-' '--' '--' '--' '--'};
  plotWidths = [.5 .5 .5 .5 1 1 1 1];
  
  for i = 1 : nSolarCells
    Ropt(i) = R(i,find(P(i,:) >= max(P(i,:))));
  end
  
  for i = 1 : nSolarCells
    if matlabMode == 'c'
      plot(R(i,:), P(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
    else
      plot(R(i,:), P(i,:))
    end
    legendStr{i} = ['Cellule ' num2str(i)];
  end
  legend(legendStr);
%   legend({'Cellule 1' 'Cellule 2' 'Cellule 3' 'Cellule 4'})
  title('Duty cycle = test%')
  
  yAxis = ylim;
  for i = 1 : nSolarCells
    text(800, yAxis(2)*(1.01 - i*.03), ['Ropt' num2str(i) ' = ' num2str(Ropt(i)) ' \Omega'])
  end
  
  Ropt
  
  % saveas(fig(nSolarCells + 1),'4cells/test','fig')
  % saveas(fig(nSolarCells + 1),'4cells/test','jpg')
  
elseif matlabMode == 'p'
  format short g
%   psoString = {'i' 'Pbest1' 'Pbest2' 'Pbest3' 'Gbest' 'v1' 'v2' 'v3'};
%   psoData;
%   close all
  % Extract data from figure
  for i = 1 : nSolarCells*2
    axesObjs = get(fig(i), 'Children');
    dataObjs = get(axesObjs, 'Children');
    xdata = get(dataObjs, 'XData');
    ydata = get(dataObjs, 'YData');
    ndata = length(xdata);

    x1 = [xdata{ndata}];
    y1 = [ydata{ndata}];
    for j = ndata - 1 : -1 : 1
      x1 = [x1 xdata{j}];
      y1 = [y1 ydata{j}];
    end

    tt(i,:) = x1;
    yy(i,:) = y1;
  end
  
  plotLines  = {'-' '-' '-' '-' '--' '--' '--' '--'};
  plotWidths = [.5 .5 .5 .5 1 1 1 1];

  fig(i+1) = figure(i +1);
  subplot(2,1,1)
  hold on
  for i = 1 : nSolarCells
%     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
    plot(tt(i,:), yy(i,:))
    legendStr{i} = ['Cellule ' num2str(i)];
  end
  legend(legendStr);
  xlabel('Itération')
  ylabel('Résistance externe [\Omega]')
  title('Évolution des résistances externes')
  
  subplot(2,1,2)
  hold on
  for i = nSolarCells + 1 : nSolarCells*2
%     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
    plot(tt(i,:), yy(i,:))
  end
  legend(legendStr);
  xlabel('Itération')
  ylabel('Puissance [W]')
  title('Évolution des puissances des cellules')
  
%   set(gcf,'units','normalized','outerposition',[0 0 1 1])
 
elseif matlabMode == 'm'
%   close all
% Extract data from figure
  for i = 1 : nSolarCells*2
    axesObjs = get(fig(i), 'Children');
    dataObjs = get(axesObjs, 'Children');
    xdata = get(dataObjs, 'XData');
    ydata = get(dataObjs, 'YData');
    ndata = length(xdata);

    x1 = [xdata{ndata}];
    y1 = [ydata{ndata}];
    for j = ndata - 1 : -1 : 1
      x1 = [x1 xdata{j}];
      y1 = [y1 ydata{j}];
    end

    tt(i,:) = x1;
    yy(i,:) = y1;
  end
  
  plotLines  = {'-' '-' '-' '-' '--' '--' '--' '--'};
  plotWidths = [.5 .5 .5 .5 1 1 1 1];

  fig(i+1) = figure(i +1);
  subplot(2,1,1)
  hold on
  for i = 1 : nSolarCells
%     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
    plot(tt(i,:), yy(i,:))
    legendStr{i} = ['Cellule ' num2str(i)];
  end
  legend(legendStr);
  xlabel('Itération')
  ylabel('Résistance externe [\Omega]')
  title('Évolution des résistances externes')
  
  subplot(2,1,2)
  hold on
  for i = nSolarCells + 1 : nSolarCells*2
%     plot(tt(i,:), yy(i,:), plotLines{i}, 'LineWidth', plotWidths(i))
    plot(tt(i,:), yy(i,:))
  end
  legend(legendStr);
  xlabel('Itération')
  ylabel('Puissance [W]')
  title('Évolution des puissances des cellules')
  
%   set(gcf,'units','normalized','outerposition',[0 0 1 1])
end

%% Steady-state

if matlabMode == 'p' || matlabMode == 'm'
  
  % Steady-state precision
  ssPrecision = 0.05;   % 5% precision for calculating steady-state
  
  nIterations = tt(end, end);

  iSteadyState = nIterations + 1;

  if oDoPerturb
    iStart = iPerturb;
  else
    iStart = 1;
  end
  
  yy = yy';

  for iData = iStart : nIterations

%     if matlabMode == 'p'
%       meanY = mean(yy(iData:end, nSolarCells + 1 : nSolarCells*2));
%     else
      meanY = mean(yy(iData:end, 1 : nSolarCells));
%     end

    for jData = iData : nIterations
%       if matlabMode == 'p'
%         if ( mean( (yy(jData, nSolarCells + 1 : nSolarCells*2) <= (1 + ssPrecision).*meanY) & (yy(jData, nSolarCells + 1 : nSolarCells*2) >= (1 - ssPrecision).*meanY) ) ~= 1 )
%           iSteadyState = nIterations + 1;
%           break;
%         else
%           iSteadyState = iData;
%         end
%       else
        if ( mean( (yy(jData, 1 : nSolarCells) <= (1 + ssPrecision).*meanY) & (yy(jData, 1 : nSolarCells) >= (1 - ssPrecision).*meanY) ) ~= 1 )
          iSteadyState = nIterations + 1;
          break;
        else
          iSteadyState = iData;
        end
%       end
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

end