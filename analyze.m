%{
count = 6;
MINK = 0.001;
MAXK = 0.00505;
STEPK = 0.00005;

MINb = 0.24;
MAXb = 2.00;
STEPb = 0.02;
%}

%{
count = 7;
MINK = 0.0001;
MAXK = 0.001;
STEPK = 0.00005;

MINb = 0.10;
MAXb = 1.00;
STEPb = 0.02;
%}

%{
count = 8;
MINK = 0.001;
MAXK = 0.002050;
STEPK = 0.00005;

MINb = 0.10;
MAXb = 1.00;
STEPb = 0.1;
%}
%{
count = 9;
MINK = 0.001;
MAXK = 0.002050;
STEPK = 0.00005;

MINb = 0.10;
MAXb = 1.50;
STEPb = 0.05;
%}

%{
count = 10;
MINK = 0.001;
MAXK = 0.003;
STEPK = 0.0005;

MINb = 0.10;
MAXb = 2.00;
STEPb = 0.1;
%}
%{
count = 12;
MINK = 0.001;
MAXK = 0.004;
STEPK = 0.0001;

MINb = 0.10;
MAXb = 2.00;
STEPb = 0.1;

%}
%{
count = 14;
MINK = 0.001;
MAXK = 0.004;
STEPK = 0.0001;

MINb = 0.10;
MAXb = 1.70;
STEPb = 0.1;
%}

%{
count = 15;
MINK = 0.001;
MAXK = 0.004;
STEPK = 0.0002;

MINb = 0.10;
MAXb = 3.00;
STEPb = 0.1;
%}
%{
count = 16;
MINK = 0.0004;
MAXK = 0.004;
STEPK = 0.0002;

MINb = 0.05;
MAXb = 2.00;
STEPb = 0.05;
%}

count = 17;
MINK = 0.0001;
MAXK = 0.0037;
STEPK = 0.0002;

MINb = 0.01;
MAXb = 2.06;
STEPb = 0.05;

%{
MINK = 0.001;
MAXK = 0.002;
STEPK = 0.00001;

MINb = 0.25;
MAXb = 1.00;
STEPb = 0.01;
%}
AK = 0;           % row containing the values of K to be plotted
Ab = 0;           % column containing the values of b to be plotted

numK = round((MAXK-MINK)/STEPK + 1);       % size of AK
numb = round((MAXb-MINb)/STEPb + 1);       % size of Ab

numK
numb
for i =1:1:numK
        AK(i) = MINK + STEPK*(i-1);       % AK ranges from MINK to MAXK in steps of STEPK
end


for i =1:1:numb
        Ab(i) = MINb + STEPb*(i-1);      % Ab ranges from MINK to MAXK in steps of STEPK
end

s1 = 0;
s2 = 0;
s3 = 0;

delay1 = 0.0;
delay2 = 0.0;
delay3 = 0.0;

K = 0.0;
b = 0.0;
p0 = 0.0;
p1 = 0.0;
p2 = 0.0;
t = 0.0;
P0 = 0;
P1 = 0;
P2 = 0;


% output of the controllers are in different files
for controller = 1:1:3 
  fname = strcat('v3/v3_controller', int2str(controller), '_17.txt'); 
  fid = fopen(fname, 'r');

  while(~feof(fid))
    s = fscanf(fid, '%f', 3);      %scanning the slot sizes from the file
    delay = fscanf(fid, '%f', 3);  %scanning the delay of this controller from the file
    
    for i=1:1:numK*numb
      K = fscanf(fid, '%f', 1);
      b = fscanf(fid, '%f', 1);
      p0 = fscanf(fid, '%f', 1);
      p1 = fscanf(fid, '%f', 1);
      p2 = fscanf(fid, '%f', 1);
      t = fscanf(fid, '%u', 1);
      
     % if(b <= MAXb && K <= MAXK) 
      m = 1 + round((K-MINK)/STEPK);
      n = 1 + round((b-MINb)/STEPb);
m
n
      P0(m, n) = p0;
      P1(m, n) = p1;
      P2(m, n) = p2;
      %end
    end
    
     if(numel(s)~=0) 
     h1 = figure; 
     fig1 = surf(Ab,AK,vec2mat(P0, round(numb)));
     xlabel('b'); ylabel('K'); zlabel('P0');
     fname = strcat('plots/v3/',int2str(count),'/figure',int2str(controller), '_P0_', int2str(round(s(1))), '_',int2str(round(s(2))), '_',int2str(round(s(3))));
     title1 = strcat('controller',int2str(controller),'\_',num2str(delay(1)), '\_',num2str(delay(2)), '\_', num2str(delay(3))); 
     title(title1);
     saveas(h1, fname, 'fig');
     saveas(h1, fname, 'jpg');   
     
     h2 = figure; 
     fig2 = surf(Ab,AK,vec2mat(P1, round(numb)));
     xlabel('b'); ylabel('K'); zlabel('P1');
     fname = strcat('plots/v3/',int2str(count),'/figure',int2str(controller), '_P1_', int2str(round(s(1))), '_',int2str(round(s(2))), '_',int2str(round(s(3))));
     title1 = strcat('controller',int2str(controller),'\_',num2str(delay(1)), '\_',num2str(delay(2)), '\_', num2str(delay(3))); 
     title(title1);
     saveas(h2, fname, 'fig');
     saveas(h2, fname, 'jpg');   
     
     h3 = figure; 
     fig3 = surf(Ab,AK,vec2mat(P2, round(numb)));
     xlabel('b'); ylabel('K'); zlabel('P2');
     fname = strcat('plots/v3/',int2str(count),'/figure',int2str(controller), '_P2_', int2str(round(s(1))), '_',int2str(round(s(2))), '_',int2str(round(s(3))));
     title1 = strcat('controller',int2str(controller),'\_',num2str(delay(1)), '\_',num2str(delay(2)), '\_', num2str(delay(3))); 
     title(title1);
     saveas(h3, fname, 'fig');
     saveas(h3, fname, 'jpg');  
     end
    close all
  end
  fclose(fid);
end

close all