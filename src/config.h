digital_mux twddle_mux = { 8, &PORTB, 1, 3, &PORTB, 5, 2, { 0, 0 } };
encoder_set twddle_enc = { &twddle_mux, 0, { 0 }, 1, 4, { 16 }, 16 };
