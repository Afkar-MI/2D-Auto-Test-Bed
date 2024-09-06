function spectrumAnalyzer = specAnalyzer_init()
    instrreset; % Reset instrument connections
    spectrumAnalyzer = visa('NI', 'USB0::0x0957::0xFFEF::CN06042405::0::INSTR'); % Adjust VISA address accordingly
    fopen(spectrumAnalyzer);

    % Set up measurement data format
    fprintf(spectrumAnalyzer, 'empty');
    centerFreq = 10.7e9; % 10.7 GHz  
    startFreq  = 10.6e9;
    endFreq    = 10.8e9;
    
    fprintf(spectrumAnalyzer, [':SENSe:FREQuency:STARt ', num2str(startFreq)]);
    fprintf(spectrumAnalyzer, [':SENSe:FREQuency:STOP ', num2str(endFreq)]);
    fprintf(spectrumAnalyzer, [':SENSe:FREQuency:CENTer ', num2str(centerFreq)]);
    fprintf(spectrumAnalyzer, ':CALC:MARK1:MODE POS');
    fprintf(spectrumAnalyzer, ':CALC:MARK1:CENter');
    fprintf(spectrumAnalyzer, ':CALC:MARK1:STAT ON');
end


