function [x,f] = shipping(values);

%setup objective function from values
objectiveFunction = repmat(values,1,3)*-1/2;

%setup intcon for intlinprog call
intcon = [1,2,3,4,5,6,7,8,9,10,11,12];

A = [
1,1,1,1,0,0,0,0,0,0,0,0; %weight in each area
0,0,0,0,1,1,1,1,0,0,0,0;
0,0,0,0,0,0,0,0,1,1,1,1;
35,50,42.5,30,0,0,0,0,0,0,0,0; %space in each area
0,0,0,0,35,50,42.5,30,0,0,0,0;
0,0,0,0,0,0,0,0,35,50,42.5,30;
1,0,0,0,1,0,0,0,1,0,0,0; %amount of each item
0,1,0,0,0,1,0,0,0,1,0,0;
0,0,1,0,0,0,1,0,0,0,1,0;
0,0,0,1,0,0,0,1,0,0,0,1;
];

%solutions of inequalities
b = [24;36;20;1000;1300;700;40;32;50;26];

%ensure ratios are correct
Aeq = [
1/12,1/12,1/12,1/12,-1/18,-1/18,-1/18,-1/18,0,0,0,0;
1/12,1/12,1/12,1/12,0,0,0,0,-1/10,-1/10,-1/10,-1/10;
];

beq = [0,0];

%set up lower bound, upper bound not relevant
lb = [0,0,0,0,0,0,0,0,0,0,0,0];
ub = [];

%call to intlinprog
[x,f,~,~] = intlinprog(objectiveFunction,intcon,A,b,Aeq,beq,lb,ub);

f = f*-1;
x = x/2;

x = vec2mat(x,4);
end