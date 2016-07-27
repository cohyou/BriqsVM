namespace gimiqs {

    /*
    |                   L                   |                   H                   |
    |        LL         |        LH         |        HL         |        HH         |
    |   LLL   |   LLH   |   LHL   |   LHH   |   HLL   |   HLH   |   HHL   |   HHH   |
    |LLLL|LLLH|LLHL|LLHH|LHLL|LHLH|LHHL|LHHH|HLLL|HLLH|HLHL|HLHH|HHLL|HHLH|HHHL|HHHH|
    */

    struct Briq {
        union {
            long L;
            struct { union { int LL;
                             struct { union { short LLL;
                                              struct { char LLLL, LLLH; }; };
                                      union { short LLH;
                                              struct { char LLHL, LLHH; }; }; }; };
                     union { int LH;
                             struct { union { short LHL;
                                              struct { char LHLL, LHLH; }; };
                                      union { short LHH;
                                              struct { char LHHL, LHHH; }; }; }; }; }; };
        union {
            long H;
            struct { union { int HL;
                             struct { union { short HLL;
                                              struct { char HLLL, HLLH; }; };
                                      union { short HLH;
                                              struct { char HLHL, HLHH; }; }; }; };
                     union { int HH;
                             struct { union { short HHL;
                                              struct { char HHLL, HHLH; }; };
                                      union { short HHH;
                                              struct { char HHHL, HHHH; }; }; }; }; }; };
    }; // struct Briq

} // namespace gimiqs
