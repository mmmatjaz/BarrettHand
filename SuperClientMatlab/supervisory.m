s=udp('193.2.6.112',4444);
% r=udp('193.2.6.112', 5555, 'LocalPort', 5555 );
fopen(s);
% fopen(r);
% SuperModeSelector=double(4);
% Buffer=typecast(x,'uint8');
Commands='@gc@3fget'
% Buffer=strcat(Buffer,Commands);
% disp(Buffer)
fwrite(s,Commands)
fscanf(s)
% fclose(r);
fclose(s);