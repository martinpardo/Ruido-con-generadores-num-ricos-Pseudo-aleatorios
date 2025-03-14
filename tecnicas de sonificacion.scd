// Tecnicas de Sonificacion del Handbook of Sonification///

(
SynthDef(\pulse, { // create a synth definition named “pulse”
      |freq = 440, amp = 0.1| // controls that can be set at runtime
      Out.ar( // create an outlet for the sound
       0, // on channel 0 (left)
       Pulse.ar( // play a pulsing signal
      freq // with the given frequency
     ) * amp // multiply it by the amp factor to determine its volume
   );
 }).add; // add it to the pool of SynthDefs
)




 x = Synth(\pulse);

 x.set(\freq, 936.236); // set the frequency of the Synth

x.free;

 a = [ 191.73, 378.39, 649.01, 424.49, 883.94, 237.32, 677.15, 812.15 ];

(
Task {
 // instantiate synth
      x = Synth(\pulse, [\freq, 20, \amp, 0]);
      0.1.wait;

     x.set(\amp, 0.1); // turn up volume
     // step through the array
    a.do{|item| // go through each item in array a
 // set freq to current value
    x.set(\freq, item);

 // wait 0.1 seconds
    0.1.wait;
};

 // remove synth
 x.free;
 }.play;
)

(
SynthDef(\sinegrain, {
                 |out = 0, attack = 0.01, decay = 0.01, freq, pan = 0, amp = 0.5|

                  var sound, env;

 // an amplitude envelope with fixed duration
                  env = EnvGen.ar(Env.perc(attack, decay), doneAction: 2);

 // the underlying sound
                  sound = FSinOsc.ar(freq);

 // use the envelope to control sound amplitude:
                  sound = sound * (env * amp);

 // add stereo panning
                  sound = Pan2.ar(sound, pan);

 // write to output bus
                  Out.ar(out, sound)
                  }).add;

)

 Synth.grain(\sinegrain, [\freq, 4040, \pan, 1.0.rand2]);


(

Task {
 // step through the array

         a.do{|item|
 // create synth with freq parameter set to current value
 // and set decay parameter to slightly overlap with next grain
        Synth.grain(\sinegrain, [\freq, item, \attack, 0.001, \decay, 0.2]);

        0.1.wait; // wait 0.1 seconds between grain onsets
         };
 }.play;

)

(

b = Buffer.loadCollection(
 server: s,
 collection: a,
 numChannels: 1,
 action: {"load completed".inform}
 );
)

(
 SynthDef(\bufferSon, {|out = 0, buf = 0, rate = 1, t_trig = 1, amp = 0.5|
                       var value, synthesis;

                       value = PlayBuf.ar(
                        numChannels: 1,
                        bufnum: buf,
                        rate: rate/SampleRate.ir,
                        trigger: t_trig,
                        loop: 0
                        );

                        synthesis = Saw.ar(value);

                      // write to outbus
                        Out.ar(out, synthesis * amp);
                        }).add;

                        x = Synth(\bufferSon, [\buf, b])

                        x.set(\rate, 5000); // set rate in samples per second
                        x.set(\t_trig, 1); // start from beginning
                        x.free; // free the synthesis process

)


(
SynthDef(\bufferAud, {|out = 0, buf = 0, rate = 1, t_trig = 1, amp = 0.5|

                      var synthesis = PlayBuf.ar(
                      numChannels: 1,
                      bufnum: buf,
                      rate: rate/SampleRate.ir,
                      trigger: t_trig,
                      loop: 0
                      );

                       // write to output bus
                      Out.ar(out, synthesis * amp)
                      }).add;


)



a = {|i|cos(i**(sin(0.0175*i*i)))}!10000;

//a.plot2; // show a graphical representation;
(
b = Buffer.loadCollection(
                       server: s,
                       collection: a,
                       numChannels: 1,
                       action: {"load completed".inform}
                      );

                      // create synth
                      x = Synth(\bufferAud, [\buf, b, \rate, 44100]);
                      x.set(\t_trig, 1); // restart
                      x.set(\rate, 200); // adjust rate
                      x.set(\t_trig, 1, \rate, 400); // restart with adjusted rate
                      x.set(\t_trig, 1, \rate, 1500);
                      x.free;

)
 (instrument: \sinegrain, freq: 4040, pan: 1.0.rand2).play

//a = [ 191.73, 378.39, 649.01, 424.49, 883.94, 237.32, 677.15, 812.15 ];

(
Pbind(
              \instrument, \sinegrain,
              \freq, Pseq( a ), // a sequence of the dataset a
              \attack, 0.001, // and fixed values as desired
              \decay, 0.2, // for the other parameters
              \dur, 0.1
              ).play

)

a = CSVFileReader.readInterpret("C:\Users\43552\Documents\SuperCollider\data.csv");
a.postcs;

Platform.currentDir.postln;



(

Pbind(
              \instrument, \sinegrain,
              \freq, Pseq( a ), // a sequence of the data (a)
              \attack, 0.001,
              \decay, 0.2,
              \pan, [-1, 1], // pan first channel to left output, second to right
              \dur, 0.1
             ).play


)

b = a.flop;

(
 Pbind(
 \instrument, \sinegrain,
 \freq, Pseq([b[1], b[2]].flop, 2),
 \attack, 0.002,
 \decay, Pseq(b[3] * 0.1, inf),
 \pan, Pseq(b[0], inf),
 \dur, 0.1
 ).play
)


 // store data
 a.writeArchive(path);
 // read data
 a = Object.readArchive(C:/Users/43552/Documents/SuperCollider);






