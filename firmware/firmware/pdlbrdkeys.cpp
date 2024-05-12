#include "pdlbrdkeys.h"

const char* PDLBRD_KEY_STR[] = { "C1", "C#1", "D1", "D#1", "E1", "F1", "F#1", "G1", "G#1", "A1", "A#1", "B1",
                                 "C2", "C#2", "D2", "D#2", "E2", "F2", "F#2", "G2", "G#2", "A2", "A#2", "B2",
								 "C3", "C#3", "D3", "NO_KEY_PRESSED"
};


const pdlbrd_key_t PDLBRD_KEY_MAP[PDLBRD_NB_SEG][NB_PIN] = {{ PDLBRD_KEY_C1, PDLBRD_KEY_CS1, PDLBRD_KEY_D1, PDLBRD_KEY_DS1, PDLBRD_KEY_E1, PDLBRD_KEY_F1, PDLBRD_KEY_FS1, PDLBRD_KEY_G1  },
                                                            { PDLBRD_KEY_GS1, PDLBRD_KEY_A1, PDLBRD_KEY_AS1, PDLBRD_KEY_B1, PDLBRD_KEY_C2, PDLBRD_KEY_CS2, PDLBRD_KEY_D2, PDLBRD_KEY_DS2 },
                                                            { PDLBRD_KEY_E2, PDLBRD_KEY_F2, PDLBRD_KEY_FS2, PDLBRD_KEY_G2, PDLBRD_KEY_GS2, PDLBRD_KEY_A2, PDLBRD_KEY_AS2, PDLBRD_KEY_B2  },
                                                            { PDLBRD_KEY_C3, PDLBRD_KEY_CS3, PDLBRD_KEY_D3, PDLBRD_NO_KEY_PRESSED, PDLBRD_NO_KEY_PRESSED, PDLBRD_NO_KEY_PRESSED, PDLBRD_NO_KEY_PRESSED, PDLBRD_NO_KEY_PRESSED }
};