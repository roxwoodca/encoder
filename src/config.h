digital_in digital_ins[] = { { 12, 0 }, { 13, 0 } };
digital_out   digital_outs[] = { { 9, 0 }, { 10, 0 }, { 11, 0} };
analog_in analog_ins[] = { { A3, 127, 1000, 100, 127, 0 } };
analog_out analog_outs[] = {};

digital_mux twddle_mux = { 12, 2, { 9, 10, 11 }, { 12, 13 }, { 0, 0 } };
encoder_set twddle_enc = { &twddle_mux, 0, { 0 }, 1, 4, { 16 }, 16 };
