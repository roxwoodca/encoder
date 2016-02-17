analog_in analog_ins[] = { { A3, 127, 1000, 100, 127, 0 } };
analog_out analog_outs[] = {};

digital_mux twddle_mux = { 8, &PORTB, 1, 3, &PORTB, 5, 2, { 0, 0 } };
encoder_set twddle_enc = { &twddle_mux, 0, { 0 }, 1, 4, { 16 }, 16 };
