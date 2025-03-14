// SynthDef para la sonificación de ruido y análisis espectral

s.options.numControlBusChannels = 1024;
s.reboot;

(
SynthDef(\noiseSonificationFFT, { |out=0, freq=440, amp=0.1, pan=0, noiseType=0, duration=0.1, fftBus=0|
    var sig, env, fft, fftChain, magnitudes;
    env = EnvGen.kr(Env.perc(0.01, duration), doneAction: 2);
    sig = Select.ar(noiseType, [
        WhiteNoise.ar,
        PinkNoise.ar,
        BrownNoise.ar,
        GrayNoise.ar,
        Dust.ar(freq),
        LFNoise0.ar(freq),
        LFNoise1.ar(freq),
        LFNoise2.ar(freq)
    ]);
    sig = sig * env * amp;
    sig = Pan2.ar(sig, pan);

    // Análisis FFT
    fft = FFT(LocalBuf(512), sig[0]);
    fftChain = PV_MagSquared(fft);

    // Obtener las magnitudes de la FFT
    magnitudes = IFFT(fftChain);

    // Enviar magnitudes FFT al bus de control
    Out.kr(fftBus, magnitudes);

    // Salida de la señal original
    Out.ar(out, sig);
}).add;
)


(
~generatePRNG = { |type, size=100, seed=1234|
    var sequence;
    switch(type,
        \linearCongruential, {
            var a = 1664525, c = 1013904223, m = 2.pow(32);
            sequence = size.collect({ |i|
                seed = (a * seed + c) % m;
                seed / m;
            });
        },
        \middleSquare, {
            sequence = size.collect({ |i|
                seed = (seed.squared.asString.padLeft(8, "0")[2..5].asInteger);
                seed / 9999;
            });
        },
        \xorshift, {
            sequence = size.collect({ |i|
                seed = seed + (seed << 13);
                seed = seed + (seed >> 17);
                seed = seed + (seed << 5);
                seed.abs / 2.pow(32);
            });
        },
        { sequence = size.collect({ 1.0.rand }); } // Método por defecto
    );
    sequence;
};
)

// Configuración de OSC
(
~oscOut = NetAddr("127.0.0.1", 12000);  // Puerto donde escucha Processing
)

// Función para sonificar secuencias PRNG y enviar datos por OSC
(
~sonifyPRNGWithFFT = { |sequence, duration=5|
    var waitTime = duration / sequence.size;
    var fftBus = Bus.control(s, 512);

    Routine({
		loop {
        sequence.do({ |value, i|
            var noiseType = (value * 8).floor;
            var freq = value.linexp(0, 1, 100, 2000);
            var amp = value.linlin(0, 1, 0.05, 0.2);
            var pan = i.linlin(0, sequence.size-1, -0.7, 0.7);

            Synth(\noiseSonificationFFT, [
                \freq, freq,
                \amp, amp,
                \pan, pan,
                \noiseType, noiseType,
                \duration, waitTime,
                \fftBus, fftBus.index
            ]);

            // Esperar un poco para que el FFT se calcule
            (waitTime * 0.9).wait;

            // Leer y enviar datos FFT por OSC
            fftBus.getn(512, { |data|
              ~oscOut.sendMsg("/fftData/part1", *data[0..255]);
              ~oscOut.sendMsg("/fftData/part2", *data[256..511]);
            });

            (waitTime * 0.1).wait;
        });
	}
    }).play;
};
)

// Generar y sonificar una secuencia PRNG
(
~prngSequence = ~generatePRNG.(\linearCongruential, 200);
~sonifyPRNGWithFFT.(~prngSequence, 10);
)











