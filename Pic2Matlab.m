function Pic2Matlab()

close all

% Create a serial port object.
port = instrfind('Type', 'serial', 'Port', 'COM5', 'Tag', '');

if isempty(port)
    port = serial('COM5');
else
    fclose(port);
    port = port(1)
end

port.BytesAvailableFcnCount = 32*4; % 32 floats to receive = 128 bytes
port.BytesAvailableFcnMode = 'byte';
port.BytesAvailableFcn = {@myCallback};

port.BaudRate = 115200;
port.DataBits = 8;
port.Parity = 'none';
port.StopBits = 1;
port.Terminator = '';

% Connect to instrument object, port.
fopen(port);

% Create figure for data
fig1 = figure(1);
hold on

% Wait for the user to stop the program
reply = 'Y'
while reply == 'Y'
  reply = input('Do you want more? Y/N [Y]:','s');
  if isempty(reply)
    reply = 'Y';
  end
end

% Disconnect from instrument object, obj1.
fclose(port);

% Clean up all objects.
delete(port);

% Extract data from figure
axesObjs = get(fig1, 'Children');
dataObjs = get(axesObjs, 'Children');
xdata = get(dataObjs, 'XData');
ydata = get(dataObjs, 'YData');
ndata = length(xdata);

x1 = [xdata{ndata}];
y1 = [ydata{ndata}];
for i = ndata - 1 : -1 : 1
  x1 = [x1 xdata{i}];
  y1 = [y1 ydata{i}];
end

figure(2)
plot(x1, y1)
legend(['Data'])

end % main function


function myCallback(obj,event)

bytesAvailable = obj.BytesAvailable;
rawData = fread(obj, bytesAvailable);
valuesReceived = obj.ValuesReceived;

% Convert UINT8 to float
data = typecast(typecast(uint8(rawData), 'uint32'), 'single');
nData = length(data);

% Add data to figure
fig1 = figure(1);
plot(data(1:nData/2), data(nData/2 + 1:nData), 'b')

end % myCallback