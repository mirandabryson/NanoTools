
float muonFakeRate(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.299;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.307;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.390;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.096;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.110;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.084;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.047;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.079;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.039;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.053;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.041;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.058;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.057;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.059;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.113;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.045;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.070;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.114;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.073;
    if (pt >= 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.059;
    if (pt >= 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.071;
    return 0.0;
}

float muonFakeRateError(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.025;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.034;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.071;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.012;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.019;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.037;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.008;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.015;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.029;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.007;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.009;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.026;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.008;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.011;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.048;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.009;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.017;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.068;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.017;
    if (pt >= 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.026;
    if (pt >= 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.102;
    return 0.0;
}

float muonHeavyFakeRate(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.296;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.319;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.389;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.093;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.110;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.084;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.046;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.079;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.040;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.053;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.036;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.059;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.053;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.056;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.100;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.042;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.051;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.118;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.071;
    if (pt >= 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.052;
    if (pt >= 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.071;
    return 0.0;
}

float muonHeavyFakeRateError(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.025;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.035;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.075;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.012;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.019;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.037;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.008;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.015;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.030;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.007;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.009;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.026;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.007;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.011;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.047;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.009;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.015;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.069;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.017;
    if (pt >= 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.025;
    if (pt >= 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.102;
    return 0.0;
}

float muonLightFakeRate(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.444;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.400;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.174;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.071;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.000;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.043;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.067;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.067;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.125;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.667;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.000;
    if (pt >= 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    return 0.0;
}

float muonLightFakeRateError(float pt, float eta) {
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.206;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 10.000 && pt < 15.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.278;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.099;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.102;
    if (pt >= 15.000 && pt < 20.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.000;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 20.000 && pt < 25.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.064;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.096;
    if (pt >= 25.000 && pt < 35.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.096;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 35.000 && pt < 50.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.168;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.346;
    if (pt >= 50.000 && pt < 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    if (pt >= 70.000 && fabs(eta) >= 0.000 && fabs(eta) < 1.200) return 0.000;
    if (pt >= 70.000 && fabs(eta) >= 1.200 && fabs(eta) < 2.100) return 0.000;
    if (pt >= 70.000 && fabs(eta) >= 2.100 && fabs(eta) < 2.400) return 0.000;
    return 0.0;
}
