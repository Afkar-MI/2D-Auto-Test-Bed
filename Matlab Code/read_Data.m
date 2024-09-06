function peakValue = read_Data(visaObj)    
    fprintf(visaObj, ':CALC:MARK1:Y?');
    peakValue = str2double(fscanf(visaObj));
end
