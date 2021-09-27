# ofdm_cpp
Example easy cpp OFDM transirver

It is full OFDM example transmitter without external library, and full OFDM example recirver with use QVector library;

Example to use and tests in class:OFDM_Window;

This exampe use for create: [Lib_myOFDM](https://github.com/Sanya-123/Lib_myOFDM)

OFDM frame include : [FetstSync;SecondSync;Header;Payload[0];Payload[1]...Payload[N]];

Slosts Header and Payload - is FFT256 frame;
Header use BPSK mod, Payload use [BPSK, QPSK, QAM16, QAM64, QAM256] mod;

(now dont use)Header must include type modulation of Payload, and size Payload slots;


# TODO
