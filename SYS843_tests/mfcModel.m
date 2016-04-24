function [dx, Pout]= mfcModel(T, x, dummy, S0, Rext)
F = 96485;
R = 8.314472;
T = 298.15;
Y = 22.753;
Ych4 = .3;
qmax_a = 8.48;
qmax_m = 8.2;
umax_a = 1.9753;
umax_m = .1;
Ks_a = 20;
Ks_m = 80;
m = 2;
gamma = 663400;
Mtotal = .05;
Km = .2*Mtotal;
Kd_a = .02*umax_a;
Kd_m = .02*umax_m;
Xmax_a = 512.5;
Xmax_m = 537.5;
Kx = .04;
Rmin = 25;
Rmax = 2000 + Rmin;
Emin = .01;
Emax = 0.25 + 0.4144+ Emin;
Kr = .024;

i0ref=0.4*4;

e = 0.0001*Mtotal;

V = 0.05;
Fin = 0.1527;

D = Fin/V;

S   = x(1);
xa  = x(2);
xm  = x(3);
Mox = x(4);

%% Équations dynamiques
  
Mred = Mtotal - Mox;

Rint = Rmin + (Rmax - Rmin) * exp(-Kr*xa);                          % (22)
Eocv = Emin + (Emax - Emin) * exp(-1/(Kr*xa));                      % (23)

alpha_a = 1/2*(1 + tanh(Kx*(xa + xm - Xmax_a)));                    % (9)
alpha_m = 1/2*(1 + tanh(Kx*(xa + xm - Xmax_m)));                    % (9)

qa = qmax_a * S / (Ks_a + S) * Mox / (Km + Mox);                    % (15)
qm = qmax_m * S / (Ks_m + S);                                       % (16)

ua = umax_a * S / (Ks_a + S) * Mox / (Km + Mox);                    % (13)
um = umax_m * S / (Ks_m + S);                                       % (14)

n_conc = R*T/F/m * log(Mtotal/Mred);                                % (18)

Imfc = (Eocv - n_conc) / (Rext + Rint) * Mred / (e + Mred);         % (21)

Sp = -qa*xa - qm*xm + D*(S0 - S);                                   % (6)
Xap = ua*xa - Kd_a*xa - alpha_a*D*xa;                               % (7)
Xmp = um*xm - Kd_m*xm - alpha_m*D*xm;                               % (8)

Moxp = -Y*qa + gamma*Imfc / (m*F*V*xa) * 86400;                     % (12)

if xa < 1 && Xap < 0
  Xap = 0;
end

if xm < 0.99 && Xmp < 0
  Xmp = 0;
end

Pout = Rext*Imfc^2;

dx = [Sp; Xap; Xmp; Moxp];
