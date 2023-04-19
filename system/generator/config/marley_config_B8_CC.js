
// Use this example configuration file as a starting point for your own files.
{
  target: {
    nuclides: [ 1000180400 ],
    atom_fractions: [ 1.0 ],
  },

  // Reaction matrix element files
  reactions: [ "ve40ArCC_Bhattacharya2009.react" ],

  // Neutrino source specification
  source: {
    type: "histogram",
    neutrino: "ve",

    // Low edges of energy bins (MeV)
    E_bin_lefts: [  0.04,   0.06,   0.08,   0.1 ,   0.12,   0.14,   0.16,   0.18,
       0.2 ,   0.22,   0.24,   0.26,   0.28,   0.3 ,   0.32,   0.34,
       0.36,   0.38,   0.4 ,   0.42,   0.44,   0.46,   0.48,   0.5 ,
       0.52,   0.54,   0.56,   0.58,   0.6 ,   0.62,   0.64,   0.66,
       0.68,   0.7 ,   0.72,   0.74,   0.76,   0.78,   0.8 ,   0.82,
       0.84,   0.86,   0.88,   0.9 ,   0.92,   0.94,   0.96,   0.98,
       1.  ,   1.02,   1.04,   1.06,   1.08,   1.1 ,   1.12,   1.14,
       1.16,   1.18,   1.2 ,   1.22,   1.24,   1.26,   1.28,   1.3 ,
       1.32,   1.34,   1.36,   1.38,   1.4 ,   1.42,   1.44,   1.46,
       1.48,   1.5 ,   1.52,   1.54,   1.56,   1.58,   1.6 ,   1.62,
       1.64,   1.66,   1.68,   1.7 ,   1.72,   1.74,   1.76,   1.78,
       1.8 ,   1.82,   1.84,   1.86,   1.88,   1.9 ,   1.92,   1.94,
       1.96,   1.98,   2.  ,   2.02,   2.04,   2.06,   2.08,   2.1 ,
       2.12,   2.14,   2.16,   2.18,   2.2 ,   2.22,   2.24,   2.26,
       2.28,   2.3 ,   2.32,   2.34,   2.36,   2.38,   2.4 ,   2.42,
       2.44,   2.46,   2.48,   2.5 ,   2.52,   2.54,   2.56,   2.58,
       2.6 ,   2.62,   2.64,   2.66,   2.68,   2.7 ,   2.72,   2.74,
       2.76,   2.78,   2.8 ,   2.82,   2.84,   2.86,   2.88,   2.9 ,
       2.92,   2.94,   2.96,   2.98,   3.  ,   3.02,   3.04,   3.06,
       3.08,   3.1 ,   3.12,   3.14,   3.16,   3.18,   3.2 ,   3.22,
       3.24,   3.26,   3.28,   3.3 ,   3.32,   3.34,   3.36,   3.38,
       3.4 ,   3.42,   3.44,   3.46,   3.48,   3.5 ,   3.52,   3.54,
       3.56,   3.58,   3.6 ,   3.62,   3.64,   3.66,   3.68,   3.7 ,
       3.72,   3.74,   3.76,   3.78,   3.8 ,   3.82,   3.84,   3.86,
       3.88,   3.9 ,   3.92,   3.94,   3.96,   3.98,   4.  ,   4.02,
       4.04,   4.06,   4.08,   4.1 ,   4.12,   4.14,   4.16,   4.18,
       4.2 ,   4.22,   4.24,   4.26,   4.28,   4.3 ,   4.32,   4.34,
       4.36,   4.38,   4.4 ,   4.42,   4.44,   4.46,   4.48,   4.5 ,
       4.52,   4.54,   4.56,   4.58,   4.6 ,   4.62,   4.64,   4.66,
       4.68,   4.7 ,   4.72,   4.74,   4.76,   4.78,   4.8 ,   4.82,
       4.84,   4.86,   4.88,   4.9 ,   4.92,   4.94,   4.96,   4.98,
       5.  ,   5.02,   5.04,   5.06,   5.08,   5.1 ,   5.12,   5.14,
       5.16,   5.18,   5.2 ,   5.22,   5.24,   5.26,   5.28,   5.3 ,
       5.32,   5.34,   5.36,   5.38,   5.4 ,   5.42,   5.44,   5.46,
       5.48,   5.5 ,   5.52,   5.54,   5.56,   5.58,   5.6 ,   5.62,
       5.64,   5.66,   5.68,   5.7 ,   5.72,   5.74,   5.76,   5.78,
       5.8 ,   5.82,   5.84,   5.86,   5.88,   5.9 ,   5.92,   5.94,
       5.96,   5.98,   6.  ,   6.02,   6.04,   6.06,   6.08,   6.1 ,
       6.12,   6.14,   6.16,   6.18,   6.2 ,   6.22,   6.24,   6.26,
       6.28,   6.3 ,   6.32,   6.34,   6.36,   6.38,   6.4 ,   6.42,
       6.44,   6.46,   6.48,   6.5 ,   6.52,   6.54,   6.56,   6.58,
       6.6 ,   6.62,   6.64,   6.66,   6.68,   6.7 ,   6.72,   6.74,
       6.76,   6.78,   6.8 ,   6.82,   6.84,   6.86,   6.88,   6.9 ,
       6.92,   6.94,   6.96,   6.98,   7.  ,   7.02,   7.04,   7.06,
       7.08,   7.1 ,   7.12,   7.14,   7.16,   7.18,   7.2 ,   7.22,
       7.24,   7.26,   7.28,   7.3 ,   7.32,   7.34,   7.36,   7.38,
       7.4 ,   7.42,   7.44,   7.46,   7.48,   7.5 ,   7.52,   7.54,
       7.56,   7.58,   7.6 ,   7.62,   7.64,   7.66,   7.68,   7.7 ,
       7.72,   7.74,   7.76,   7.78,   7.8 ,   7.82,   7.84,   7.86,
       7.88,   7.9 ,   7.92,   7.94,   7.96,   7.98,   8.  ,   8.02,
       8.04,   8.06,   8.08,   8.1 ,   8.12,   8.14,   8.16,   8.18,
       8.2 ,   8.22,   8.24,   8.26,   8.28,   8.3 ,   8.32,   8.34,
       8.36,   8.38,   8.4 ,   8.42,   8.44,   8.46,   8.48,   8.5 ,
       8.52,   8.54,   8.56,   8.58,   8.6 ,   8.62,   8.64,   8.66,
       8.68,   8.7 ,   8.72,   8.74,   8.76,   8.78,   8.8 ,   8.82,
       8.84,   8.86,   8.88,   8.9 ,   8.92,   8.94,   8.96,   8.98,
       9.  ,   9.02,   9.04,   9.06,   9.08,   9.1 ,   9.12,   9.14,
       9.16,   9.18,   9.2 ,   9.22,   9.24,   9.26,   9.28,   9.3 ,
       9.32,   9.34,   9.36,   9.38,   9.4 ,   9.42,   9.44,   9.46,
       9.48,   9.5 ,   9.52,   9.54,   9.56,   9.58,   9.6 ,   9.62,
       9.64,   9.66,   9.68,   9.7 ,   9.72,   9.74,   9.76,   9.78,
       9.8 ,   9.82,   9.84,   9.86,   9.88,   9.9 ,   9.92,   9.94,
       9.96,   9.98,  10.  ,  10.02,  10.04,  10.06,  10.08,  10.1 ,
      10.12,  10.14,  10.16,  10.18,  10.2 ,  10.22,  10.24,  10.26,
      10.28,  10.3 ,  10.32,  10.34,  10.36,  10.38,  10.4 ,  10.42,
      10.44,  10.46,  10.48,  10.5 ,  10.52,  10.54,  10.56,  10.58,
      10.6 ,  10.62,  10.64,  10.66,  10.68,  10.7 ,  10.72,  10.74,
      10.76,  10.78,  10.8 ,  10.82,  10.84,  10.86,  10.88,  10.9 ,
      10.92,  10.94,  10.96,  10.98,  11.  ,  11.02,  11.04,  11.06,
      11.08,  11.1 ,  11.12,  11.14,  11.16,  11.18,  11.2 ,  11.22,
      11.24,  11.26,  11.28,  11.3 ,  11.32,  11.34,  11.36,  11.38,
      11.4 ,  11.42,  11.44,  11.46,  11.48,  11.5 ,  11.52,  11.54,
      11.56,  11.58,  11.6 ,  11.62,  11.64,  11.66,  11.68,  11.7 ,
      11.72,  11.74,  11.76,  11.78,  11.8 ,  11.82,  11.84,  11.86,
      11.88,  11.9 ,  11.92,  11.94,  11.96,  11.98,  12.  ,  12.02,
      12.04,  12.06,  12.08,  12.1 ,  12.12,  12.14,  12.16,  12.18,
      12.2 ,  12.22,  12.24,  12.26,  12.28,  12.3 ,  12.32,  12.34,
      12.36,  12.38,  12.4 ,  12.42,  12.44,  12.46,  12.48,  12.5 ,
      12.52,  12.54,  12.56,  12.58,  12.6 ,  12.62,  12.64,  12.66,
      12.68,  12.7 ,  12.72,  12.74,  12.76,  12.78,  12.8 ,  12.82,
      12.84,  12.86,  12.88,  12.9 ,  12.92,  12.94,  12.96,  12.98,
      13.  ,  13.02,  13.04,  13.06,  13.08,  13.1 ,  13.12,  13.14,
      13.16,  13.18,  13.2 ,  13.22,  13.24,  13.26,  13.28,  13.3 ,
      13.32,  13.34,  13.36,  13.38,  13.4 ,  13.42,  13.44,  13.46,
      13.48,  13.5 ,  13.52,  13.54,  13.56,  13.58,  13.6 ,  13.62,
      13.64,  13.66,  13.68,  13.7 ,  13.72,  13.74,  13.76,  13.78,
      13.8 ,  13.82,  13.84,  13.86,  13.88,  13.9 ,  13.92,  13.94,
      13.96,  13.98,  14.  ,  14.02,  14.04,  14.06,  14.08,  14.1 ,
      14.12,  14.14,  14.16,  14.18,  14.2 ,  14.22,  14.24,  14.26,
      14.28,  14.3 ,  14.32,  14.34,  14.36,  14.38,  14.4 ,  14.42,
      14.44,  14.46,  14.48,  14.5 ,  14.52,  14.54,  14.56,  14.58,
      14.6 ,  14.62,  14.64,  14.66,  14.68,  14.7 ,  14.72,  14.74,
      14.76,  14.78,  14.8 ,  14.82,  14.84,  14.86,  14.88,  14.9 ,
      14.92,  14.94,  14.96,  14.98,  15.  ,  15.02,  15.04,  15.06,
      15.08,  15.1 ,  15.12,  15.14,  15.16,  15.18,  15.2 ,  15.22,
      15.24,  15.26,  15.28,  15.3 ,  15.32,  15.34,  15.36,  15.38,
      15.4 ,  15.42,  15.44,  15.46,  15.48,  15.5 ,  15.52,  15.54,
      15.56,  15.58,  15.6 ,  15.62,  15.64,  15.66,  15.68,  15.7 ,
      15.72,  15.74,  15.76,  15.78,  15.8 ,  15.82,  15.84,  15.86,
      15.88,  15.9 ,  15.92,  15.94,  15.96,  15.98,  16.  ,  16.02,
      16.04,  16.06,  16.08,  16.1 ],

    // Bin weights (dimensionless)
    weights: [  1.75440000e-04,   3.88032000e-04,   6.78024000e-04,
       1.04180400e-03,   1.47524400e-03,   1.97370000e-03,
       2.53459200e-03,   3.15379200e-03,   3.82717200e-03,
       4.55060400e-03,   5.32099200e-03,   6.13266000e-03,
       6.98044800e-03,   7.85352000e-03,   8.73020400e-03,
       9.62236800e-03,   1.06966800e-02,   1.18169160e-02,
       1.29804960e-02,   1.41812280e-02,   1.54268520e-02,
       1.67374920e-02,   1.80899280e-02,   1.94815800e-02,
       2.09036760e-02,   2.23747920e-02,   2.39129880e-02,
       2.54919480e-02,   2.71090920e-02,   2.87597760e-02,
       3.04563840e-02,   3.22009800e-02,   3.39827280e-02,
       3.58005960e-02,   3.76514880e-02,   3.95426280e-02,
       4.14724680e-02,   4.34358480e-02,   4.54307040e-02,
       4.74523920e-02,   4.95184560e-02,   5.16190920e-02,
       5.37491400e-02,   5.59075680e-02,   5.80850880e-02,
       6.03162720e-02,   6.25779000e-02,   6.48668760e-02,
       6.71811360e-02,   6.95134560e-02,   7.18968600e-02,
       7.43060640e-02,   7.67405520e-02,   7.91977440e-02,
       8.16745440e-02,   8.41946880e-02,   8.67380520e-02,
       8.93051520e-02,   9.18928920e-02,   9.45002400e-02,
       9.71426760e-02,   9.98062680e-02,   1.02490500e-01,
       1.05193820e-01,   1.07916240e-01,   1.10666000e-01,
       1.13434860e-01,   1.16220740e-01,   1.19022110e-01,
       1.21842050e-01,   1.24690880e-01,   1.27556750e-01,
       1.30438090e-01,   1.33331820e-01,   1.36244640e-01,
       1.39181200e-01,   1.42132200e-01,   1.45096620e-01,
       1.48072910e-01,   1.51065710e-01,   1.54077080e-01,
       1.57101360e-01,   1.60136470e-01,   1.63181390e-01,
       1.66241780e-01,   1.69316630e-01,   1.72400760e-01,
       1.75494180e-01,   1.78591730e-01,   1.81710430e-01,
       1.84841520e-01,   1.87980860e-01,   1.91126920e-01,
       1.94275550e-01,   1.97444820e-01,   2.00621830e-01,
       2.03805040e-01,   2.06993400e-01,   2.10182280e-01,
       2.13392320e-01,   2.16607510e-01,   2.19826840e-01,
       2.23049770e-01,   2.26272710e-01,   2.29513190e-01,
       2.32756760e-01,   2.36003440e-01,   2.39250620e-01,
       2.42498840e-01,   2.45761000e-01,   2.49024700e-01,
       2.52289940e-01,   2.55554160e-01,   2.58818890e-01,
       2.62094460e-01,   2.65370030e-01,   2.68645080e-01,
       2.71917550e-01,   2.75190540e-01,   2.78473330e-01,
       2.81754580e-01,   2.85033760e-01,   2.88308290e-01,
       2.91583860e-01,   2.94865100e-01,   2.98143770e-01,
       3.01417790e-01,   3.04685620e-01,   3.07956020e-01,
       3.11229530e-01,   3.14497870e-01,   3.17761060e-01,
       3.21015980e-01,   3.24274520e-01,   3.27532550e-01,
       3.30784380e-01,   3.34030020e-01,   3.37264310e-01,
       3.40504790e-01,   3.43741660e-01,   3.46971300e-01,
       3.50192170e-01,   3.53400140e-01,   3.56617400e-01,
       3.59826920e-01,   3.63028190e-01,   3.66220160e-01,
       3.69399760e-01,   3.72583480e-01,   3.75758420e-01,
       3.78923570e-01,   3.82077880e-01,   3.85220830e-01,
       3.88365340e-01,   3.91499520e-01,   3.94623380e-01,
       3.97734860e-01,   4.00834480e-01,   4.03934600e-01,
       4.07023900e-01,   4.10101320e-01,   4.13164810e-01,
       4.16217980e-01,   4.19267540e-01,   4.22304720e-01,
       4.25329000e-01,   4.28337790e-01,   4.31337300e-01,
       4.34332680e-01,   4.37314640e-01,   4.40283190e-01,
       4.43235230e-01,   4.46178490e-01,   4.49113500e-01,
       4.52034580e-01,   4.54940690e-01,   4.57828740e-01,
       4.60710080e-01,   4.63581110e-01,   4.66436650e-01,
       4.69276200e-01,   4.72095620e-01,   4.74911440e-01,
       4.77714860e-01,   4.80501260e-01,   4.83271150e-01,
       4.86018340e-01,   4.88766040e-01,   4.91497220e-01,
       4.94211380e-01,   4.96907480e-01,   4.99582430e-01,
       5.02254280e-01,   5.04909100e-01,   5.07546370e-01,
       5.10164040e-01,   5.12761070e-01,   5.15353970e-01,
       5.17928810e-01,   5.20484560e-01,   5.23020700e-01,
       5.25536710e-01,   5.28047570e-01,   5.30539330e-01,
       5.33011490e-01,   5.35463520e-01,   5.37896460e-01,
       5.40321660e-01,   5.42727250e-01,   5.45112720e-01,
       5.47476000e-01,   5.49823280e-01,   5.52159730e-01,
       5.54476060e-01,   5.56771220e-01,   5.59043690e-01,
       5.61301190e-01,   5.63545270e-01,   5.65768720e-01,
       5.67969970e-01,   5.70146460e-01,   5.72312630e-01,
       5.74462280e-01,   5.76591300e-01,   5.78697610e-01,
       5.80777090e-01,   5.82849860e-01,   5.84903540e-01,
       5.86935040e-01,   5.88943820e-01,   5.90925260e-01,
       5.92901540e-01,   5.94856150e-01,   5.96788060e-01,
       5.98696220e-01,   6.00578590e-01,   6.02454250e-01,
       6.04307720e-01,   6.06137980e-01,   6.07943460e-01,
       6.09725210e-01,   6.11497670e-01,   6.13247940e-01,
       6.14974480e-01,   6.16676240e-01,   6.18354790e-01,
       6.20023020e-01,   6.21668540e-01,   6.23289820e-01,
       6.24885800e-01,   6.26461150e-01,   6.28023600e-01,
       6.29562310e-01,   6.31077290e-01,   6.32565430e-01,
       6.34035000e-01,   6.35490120e-01,   6.36921500e-01,
       6.38328120e-01,   6.39707390e-01,   6.41070660e-01,
       6.42417420e-01,   6.43739410e-01,   6.45037150e-01,
       6.46305480e-01,   6.47561940e-01,   6.48798280e-01,
       6.50009840e-01,   6.51196640e-01,   6.52353000e-01,
       6.53501100e-01,   6.54626500e-01,   6.55726610e-01,
       6.56801440e-01,   6.57846340e-01,   6.58884010e-01,
       6.59896400e-01,   6.60884540e-01,   6.61846880e-01,
       6.62781360e-01,   6.63706030e-01,   6.64606450e-01,
       6.65481590e-01,   6.66329890e-01,   6.67152910e-01,
       6.67965610e-01,   6.68752510e-01,   6.69514640e-01,
       6.70249430e-01,   6.70960990e-01,   6.71660170e-01,
       6.72335100e-01,   6.72983710e-01,   6.73604980e-01,
       6.74206120e-01,   6.74791780e-01,   6.75353700e-01,
       6.75888790e-01,   6.76396020e-01,   6.76886220e-01,
       6.77359910e-01,   6.77808310e-01,   6.78231430e-01,
       6.78625140e-01,   6.79005430e-01,   6.79365600e-01,
       6.79701000e-01,   6.80009570e-01,   6.80288210e-01,
       6.80558590e-01,   6.80806270e-01,   6.81028150e-01,
       6.81224750e-01,   6.81390900e-01,   6.81550860e-01,
       6.81685020e-01,   6.81795440e-01,   6.81878520e-01,
       6.81934250e-01,   6.81982240e-01,   6.82005460e-01,
       6.82003390e-01,   6.81975530e-01,   6.81921350e-01,
       6.81857880e-01,   6.81769640e-01,   6.81656640e-01,
       6.81516800e-01,   6.81354260e-01,   6.81180890e-01,
       6.80982740e-01,   6.80759830e-01,   6.80509570e-01,
       6.80240220e-01,   6.79956940e-01,   6.79650430e-01,
       6.79318640e-01,   6.78958990e-01,   6.78583860e-01,
       6.78192730e-01,   6.77777870e-01,   6.77337720e-01,
       6.76869190e-01,   6.76389830e-01,   6.75891890e-01,
       6.75370210e-01,   6.74823770e-01,   6.74246360e-01,
       6.73664830e-01,   6.73061630e-01,   6.72433660e-01,
       6.71780920e-01,   6.71099280e-01,   6.70415060e-01,
       6.69707110e-01,   6.68976460e-01,   6.68220520e-01,
       6.67437740e-01,   6.66650840e-01,   6.65840720e-01,
       6.65007380e-01,   6.64148760e-01,   6.63267430e-01,
       6.62378880e-01,   6.61468140e-01,   6.60534180e-01,
       6.59574940e-01,   6.58596080e-01,   6.57607940e-01,
       6.56597620e-01,   6.55564580e-01,   6.54505750e-01,
       6.53431440e-01,   6.52345260e-01,   6.51237410e-01,
       6.50106850e-01,   6.48949980e-01,   6.47782790e-01,
       6.46601150e-01,   6.45398350e-01,   6.44172850e-01,
       6.42920000e-01,   6.41662000e-01,   6.40386960e-01,
       6.39090770e-01,   6.37771360e-01,   6.36424080e-01,
       6.35078870e-01,   6.33712500e-01,   6.32326010e-01,
       6.30916810e-01,   6.29481820e-01,   6.28047340e-01,
       6.26592220e-01,   6.25116970e-01,   6.23619540e-01,
       6.22099400e-01,   6.20577720e-01,   6.19036940e-01,
       6.17475530e-01,   6.15891410e-01,   6.14289230e-01,
       6.12683440e-01,   6.11058550e-01,   6.09413540e-01,
       6.07745830e-01,   6.06064190e-01,   6.04376350e-01,
       6.02669420e-01,   6.00943400e-01,   5.99193650e-01,
       5.97435120e-01,   5.95668340e-01,   5.93882980e-01,
       5.92078010e-01,   5.90249300e-01,   5.88418020e-01,
       5.86574870e-01,   5.84713660e-01,   5.82833350e-01,
       5.80928280e-01,   5.79027340e-01,   5.77110910e-01,
       5.75176940e-01,   5.73224400e-01,   5.71247600e-01,
       5.69277520e-01,   5.67290400e-01,   5.65286770e-01,
       5.63264050e-01,   5.61221720e-01,   5.59184040e-01,
       5.57129840e-01,   5.55059140e-01,   5.52969340e-01,
       5.50864060e-01,   5.48761360e-01,   5.46643690e-01,
       5.44509000e-01,   5.42356760e-01,   5.40192140e-01,
       5.38028040e-01,   5.35848460e-01,   5.33653390e-01,
       5.31438720e-01,   5.29218890e-01,   5.26995440e-01,
       5.24758070e-01,   5.22504700e-01,   5.20232230e-01,
       5.17959770e-01,   5.15682140e-01,   5.13390590e-01,
       5.11084070e-01,   5.08755880e-01,   5.06436460e-01,
       5.04107230e-01,   5.01764590e-01,   4.99406990e-01,
       4.97029260e-01,   4.94663920e-01,   4.92286190e-01,
       4.89895560e-01,   4.87490480e-01,   4.85068900e-01,
       4.82658140e-01,   4.80235520e-01,   4.77801040e-01,
       4.75351580e-01,   4.72890260e-01,   4.70438230e-01,
       4.67974850e-01,   4.65499600e-01,   4.63010410e-01,
       4.60513490e-01,   4.58023270e-01,   4.55523250e-01,
       4.53011360e-01,   4.50484510e-01,   4.47956630e-01,
       4.45432360e-01,   4.42898280e-01,   4.40353370e-01,
       4.37792980e-01,   4.35237740e-01,   4.32683030e-01,
       4.30118510e-01,   4.27543670e-01,   4.24952320e-01,
       4.22374900e-01,   4.19793350e-01,   4.17202510e-01,
       4.14602390e-01,   4.11984720e-01,   4.09387690e-01,
       4.06782920e-01,   4.04169900e-01,   4.01547070e-01,
       3.98911860e-01,   3.96295220e-01,   3.93671360e-01,
       3.91040280e-01,   3.88399390e-01,   3.85750760e-01,
       3.83118130e-01,   3.80479310e-01,   3.77833780e-01,
       3.75177920e-01,   3.72520010e-01,   3.69876020e-01,
       3.67225850e-01,   3.64569480e-01,   3.61902280e-01,
       3.59240230e-01,   3.56587990e-01,   3.53931110e-01,
       3.51268030e-01,   3.48593090e-01,   3.45931040e-01,
       3.43275190e-01,   3.40615210e-01,   3.37949040e-01,
       3.35269450e-01,   3.32613600e-01,   3.29958260e-01,
       3.27298800e-01,   3.24633660e-01,   3.21956140e-01,
       3.19307510e-01,   3.16656300e-01,   3.14002000e-01,
       3.11342020e-01,   3.08674300e-01,   3.06033920e-01,
       3.03391490e-01,   3.00745960e-01,   2.98095260e-01,
       2.95443020e-01,   2.92814000e-01,   2.90184470e-01,
       2.87551840e-01,   2.84914040e-01,   2.82280900e-01,
       2.79667870e-01,   2.77054850e-01,   2.74439240e-01,
       2.71817450e-01,   2.69208550e-01,   2.66616170e-01,
       2.64022750e-01,   2.61428820e-01,   2.58825600e-01,
       2.56246120e-01,   2.53677980e-01,   2.51109850e-01,
       2.48540690e-01,   2.45959660e-01,   2.43416290e-01,
       2.40876020e-01,   2.38336790e-01,   2.35796520e-01,
       2.33248000e-01,   2.30737660e-01,   2.28229900e-01,
       2.25723170e-01,   2.23215410e-01,   2.20705580e-01,
       2.18230850e-01,   2.15758690e-01,   2.13288600e-01,
       2.10816440e-01,   2.08349450e-01,   2.05913930e-01,
       2.03482540e-01,   2.01052690e-01,   1.98620270e-01,
       1.96201260e-01,   1.93809600e-01,   1.91421550e-01,
       1.89036080e-01,   1.86646490e-01,   1.84280630e-01,
       1.81936440e-01,   1.79596380e-01,   1.77258380e-01,
       1.74913680e-01,   1.72606640e-01,   1.70313540e-01,
       1.68025080e-01,   1.65738680e-01,   1.63443520e-01,
       1.61198920e-01,   1.58960510e-01,   1.56726740e-01,
       1.54495040e-01,   1.52260760e-01,   1.50074470e-01,
       1.47894370e-01,   1.45719950e-01,   1.43546560e-01,
       1.41377810e-01,   1.39253440e-01,   1.37135770e-01,
       1.35023270e-01,   1.32911280e-01,   1.30812710e-01,
       1.28753870e-01,   1.26701740e-01,   1.24655280e-01,
       1.22606240e-01,   1.20583010e-01,   1.18592800e-01,
       1.16609810e-01,   1.14631980e-01,   1.12648990e-01,
       1.10706250e-01,   1.08788280e-01,   1.06877530e-01,
       1.04970910e-01,   1.03052940e-01,   1.01197920e-01,
       9.93547680e-02,   9.75188400e-02,   9.56865240e-02,
       9.38464680e-02,   9.20734920e-02,   9.03087720e-02,
       8.85512760e-02,   8.67958440e-02,   8.50419600e-02,
       8.33494800e-02,   8.16657720e-02,   7.99887720e-02,
       7.83117720e-02,   7.66476720e-02,   7.50393000e-02,
       7.34391840e-02,   7.18447440e-02,   7.02477240e-02,
       6.86775360e-02,   6.71553360e-02,   6.56408760e-02,
       6.41315760e-02,   6.26140200e-02,   6.11434200e-02,
       5.97094560e-02,   5.82832320e-02,   5.68606200e-02,
       5.54214960e-02,   5.40571920e-02,   5.27135280e-02,
       5.13765720e-02,   5.00406480e-02,   4.86866640e-02,
       4.74271080e-02,   4.61752920e-02,   4.49296680e-02,
       4.36830120e-02,   4.24296480e-02,   4.12645200e-02,
       4.01066160e-02,   3.89528400e-02,   3.77949360e-02,
       3.66447720e-02,   3.55751040e-02,   3.45116280e-02,
       3.34507320e-02,   3.23805480e-02,   3.13382280e-02,
       3.03645360e-02,   2.93954880e-02,   2.84269560e-02,
       2.74413960e-02,   2.65115640e-02,   2.56338480e-02,
       2.47602600e-02,   2.38840920e-02,   2.29790280e-02,
       2.21683920e-02,   2.13866520e-02,   2.06069760e-02,
       1.98221400e-02,   1.89991200e-02,   1.83071640e-02,
       1.76193360e-02,   1.69320240e-02,   1.62364560e-02,
       1.55197320e-02,   1.49206560e-02,   1.43241600e-02,
       1.37266320e-02,   1.31162040e-02,   1.25062920e-02,
       1.19954520e-02,   1.14861600e-02,   1.09742880e-02,
       1.04448720e-02,   9.93919200e-03,   9.51142800e-03,
       9.08366400e-03,   8.65125600e-03,   8.19717600e-03,
       7.79211600e-03,   7.43968800e-03,   7.08622800e-03,
       6.72760800e-03,   6.33957600e-03,   6.02946000e-03,
       5.74359600e-03,   5.45670000e-03,   5.16309600e-03,
       4.83595200e-03,   4.60788000e-03,   4.37980800e-03,
       4.15070400e-03,   3.91386000e-03,   3.66050400e-03,
       3.48196800e-03,   3.30291600e-03,   3.12231600e-03,
       2.93397600e-03,   2.74357200e-03,   2.60528400e-03,
       2.46648000e-03,   2.32561200e-03,   2.17648800e-03,
       2.03716800e-03,   1.93138800e-03,   1.82457600e-03,
       1.71621600e-03,   1.59908400e-03,   1.50001200e-03,
       1.41951600e-03,   1.33850400e-03,   1.25542800e-03,
       1.16254800e-03,   1.09443600e-03,   1.03354800e-03,
       9.72144000e-04,   9.08676000e-04,   8.35920000e-04,
       7.90512000e-04,   7.45104000e-04,   6.99180000e-04,
       6.51192000e-04,   5.99076000e-04,   5.65536000e-04,
       5.31480000e-04,   4.96908000e-04,   4.60788000e-04,
       4.24152000e-04,   3.98868000e-04,   3.74100000e-04,
       3.48300000e-04,   3.20952000e-04,   2.96184000e-04,
       2.77608000e-04,   2.59032000e-04,   2.40456000e-04,
       2.19816000e-04,   2.02788000e-04,   1.89888000e-04,
       1.76472000e-04,   1.62540000e-04,   1.47060000e-04,
       1.36224000e-04,   1.26936000e-04,   1.17648000e-04,
       1.07844000e-04,   9.64920000e-05,   8.97840000e-05,
       8.35920000e-05,   7.68840000e-05,   6.96600000e-05,
       6.24360000e-05,   5.77920000e-05,   5.31480000e-05,
       4.90200000e-05,   4.38600000e-05,   3.92160000e-05,
       3.61200000e-05,   3.30240000e-05,   2.99280000e-05,
       2.68320000e-05,   2.37360000e-05,   2.16720000e-05,
       1.96080000e-05,   1.75440000e-05,   1.54800000e-05,
       1.39320000e-05,   1.23840000e-05,   1.13520000e-05,
       9.80400000e-06,   8.25600000e-06,   7.74000000e-06,
       6.70800000e-06,   6.19200000e-06,   5.16000000e-06,
       4.64400000e-06,   4.12800000e-06,   3.61200000e-06,
       3.09600000e-06,   2.58000000e-06,   2.06400000e-06],

    // Upper edge of the final bin (MeV)
    Emax: 16.12,
  },

  // Incident neutrino direction 3-vector
  direction: { x: 0.0, y: 0.0, z: 1.0 },

  log: [ { file: "stdout", level: "disabled" } ],
}

