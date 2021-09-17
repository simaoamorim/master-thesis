function tuning_analisis(filename)
    T = readtable(filename,'HeaderLines',3);
    t = table2array(T);
    x = t(:,2);
    y1 = t(:,3);
    y2 = t(:,4);
    
    L = 500;
    t = x(1:L);
    Y = y2(1:L)-mean(y2(1:L));
    
    P2 = abs(Y/L);
    P1 = P2(1:L/2+1);
    
    fftY = fft(P1);
    
    plot(abs(fftY));
    
end