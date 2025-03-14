
// SynthDef para la sonificación de ruido
// Este ejemplo de sonificación esta arreglado para que
// routine haga la secuencia con inf.do
// o sea de forma infinita
(
SynthDef(\noiseSonification, { |freq=440, amp=0.1, pan=0, noiseType=0, duration=0.1|
    var sig, env;
    env = EnvGen.kr(Env.perc(0.01, duration), doneAction: 2);
    sig = Select.ar(noiseType, [
        WhiteNoise.ar,        // Ruido blanco
        PinkNoise.ar,         // Ruido rosa
        BrownNoise.ar,        // Ruido marrón
        GrayNoise.ar,         // Ruido gris
        Dust.ar(freq),        // Ruido impulsivo
        LFNoise0.ar(freq),    // Ruido escalonado
        LFNoise1.ar(freq),    // Ruido lineal interpolado
        LFNoise2.ar(freq)     // Ruido cuadrático interpolado
    ]);
    sig = sig * env * amp;
    sig = Pan2.ar(sig, pan);
    Out.ar(0, sig);
}).add;
)

// Función para generar secuencias de PRNG
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
// Función para sonificar secuencias PRNG
(
~sonifyPRNG = { |sequence, duration=5|
    var waitTime = duration / sequence.size;

    Routine({
        inf.do({ |i|
			var value = sequence.wrapAt(i);
            var noiseType = (value * 8).floor;
            var freq = value.linexp(0, 1, 100, 2000);
            var amp = value.linlin(0, 1, 0.05, 0.2);
//            var pan = i.linlin(0, sequence.size-1, -0.7, 0.7);
             var pan = value.linlin(0, 1, -1, 1);
            Synth(\noiseSonification, [
                \freq, freq,
                \amp, amp,
                \pan, pan,
                \noiseType, noiseType,
                \duration, waitTime
            ]);
            waitTime.wait;
        });
    }).play;
};
)

// Generar y sonificar diferentes secuencias PRNG
(
~lcg = ~generatePRNG.(\linearCongruential, 200);
~ms = ~generatePRNG.(\middleSquare, 200);
~xor = ~generatePRNG.(\xorshift, 200);

// Sonificar cada secuencia
~routine = ~sonifyPRNG.(~lcg, 10);
)


~routine.stop;

// Espera unos segundos y luego ejecuta:
(
~sonifyPRNG.(~ms, 10);
)

// Espera unos segundos más y ejecuta:
(
~sonifyPRNG.(~xor, 10);
)

// Visualización de las secuencias (opcional)
(
~lcg.plot("Linear Congruential Generator");
~ms.plot("Middle Square Method");
~xor.plot("Xorshift");
)


s.prepareForRecord; // Prepara el servidor para grabar
s.record;           // Comienza la grabación

s.stopRecording;    // Detiene la grabación


