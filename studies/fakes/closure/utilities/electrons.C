
float electronFakeRate(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.251;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.153;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.202;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.069;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.094;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.082;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.056;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.048;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.046;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.027;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.038;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.035;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.033;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.040;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.043;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.048;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.062;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.087;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.100;
    if (pt >= 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.087;
    if (pt >= 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.119;
    return 0.0;
}

float electronFakeRateError(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.034;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.034;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.050;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.014;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.019;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.022;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.011;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.012;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.016;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.006;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.008;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.009;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.006;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.008;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.010;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.010;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.013;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.018;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.023;
    if (pt >= 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.025;
    if (pt >= 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.030;
    return 0.0;
}

float electronHeavyFakeRate(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.236;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.157;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.254;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.066;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.103;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.079;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.054;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.041;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.046;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.026;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.029;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.036;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.027;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.032;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.028;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.036;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.051;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.066;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.076;
    if (pt >= 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.058;
    if (pt >= 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.116;
    return 0.0;
}

float electronHeavyFakeRateError(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.035;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.038;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.062;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.014;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.022;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.026;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.011;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.013;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.019;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.006;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.008;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.011;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.006;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.008;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.010;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.010;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.014;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.021;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.023;
    if (pt >= 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.028;
    if (pt >= 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.046;
    return 0.0;
}

float electronLightFakeRate(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.571;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.050;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.000;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.100;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.053;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.061;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.059;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.055;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.033;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.014;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.042;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.011;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.039;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.057;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.056;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.079;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.059;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.097;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.189;
    if (pt >= 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.130;
    if (pt >= 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.077;
    return 0.0;
}

float electronLightFakeRateError(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.235;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.074;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.000;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.091;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.050;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.045;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.055;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.040;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.031;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.021;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.023;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.016;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.024;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.023;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.024;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.034;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.028;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.031;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 0.800) return 0.077;
    if (pt >= 70.000 && fabs(eta) >= 0.800 && fabs(eta) < 1.470) return 0.055;
    if (pt >= 70.000 && fabs(eta) >= 1.470 && fabs(eta) < 2.500) return 0.037;
    return 0.0;
}
