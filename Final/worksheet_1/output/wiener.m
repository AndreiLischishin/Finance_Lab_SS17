fid = fopen('wiener_process_points_0.5.txt', 'r');format_spec = '%f %f %f %f';sizeA = [4 Inf];A = fscanf(fid, format_spec,sizeA);fclose(fid);A = A';n = A(:,1);x = A(:,2);y = A(:,3);z = A(:,4);plot(n,x);hold onplot(n,y);hold onplot(n,z);#title('estimation of sigma with mu = 0');#legend('sigma = 0.1', 'sigma = 1','sigma = 10');#ylabel('estimation error');#xlabel('N');