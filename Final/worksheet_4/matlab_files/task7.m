fid = fopen('../output/task7.txt', 'r');
format_spec = '%f %f %f %f %f';
sizeA = [5 Inf];
A = fscanf(fid, format_spec, sizeA);
fclose(fid);
A=A';
n = A(:,1);
x = A(:,2);
y = A(:,3);
z = A(:,4);
u = A(:,5);

plot(log10(n), x,'bo--','Linewidth',2);

hold on

plot(log10(n), y,'r^-','Linewidth',2);

hold on

plot(log10(n), z,':ms','Linewidth',2);

hold on

plot(log10(n), u,'-g*','Linewidth',2);



title('M=64, asian call, arithmetic, control variates, QMC, MC');
ylabel('error(N)');
xlabel('N');
lngd = legend('QMC BB','QMC BB CV','MC BB','MC BB CV');
set(lngd,'fontsize',4,'Position',[0.6,0.15,0.3,0.1]);
