{
	//https://arxiv.org/abs/1910.11878 Used for flux HEP
seed : 123456,
reactions: [ "ve40ArCC_Bhattacharya2009.react" ],
source:{
    type: "histogram",
    neutrino: "ve",
    E_bin_lefts: [
	0.018784 ,  0.037567 ,  0.056351 ,  0.075135 ,  0.093919 ,  0.1127 ,  0.13149 ,  0.15027 ,  0.16905 ,  0.18784 ,
	0.20662 ,  0.2254 ,  0.24419 ,  0.26297 ,  0.28176 ,  0.30054 ,  0.31932 ,  0.33811 ,  0.35689 ,  0.37567 ,
	0.39446 ,  0.41324 ,  0.43203 ,  0.45081 ,  0.46959 ,  0.48838 ,  0.50716 ,  0.52594 ,  0.54473 ,  0.56351 ,
	0.5823 ,  0.60108 ,  0.61986 ,  0.63865 ,  0.65743 ,  0.67621 ,  0.695 ,  0.71378 ,  0.73257 ,  0.75135 ,
	0.77013 ,  0.78892 ,  0.8077 ,  0.82648 ,  0.84527 ,  0.86405 ,  0.88284 ,  0.90162 ,  0.9204 ,  0.93919 ,
	0.95797 ,  0.97675 ,  0.99554 ,  1.0143 ,  1.0331 ,  1.0519 ,  1.0707 ,  1.0895 ,  1.1082 ,  1.127 ,
	1.1458 ,  1.1646 ,  1.1834 ,  1.2022 ,  1.2209 ,  1.2397 ,  1.2585 ,  1.2773 ,  1.2961 ,  1.3149 ,
	1.3336 ,  1.3524 ,  1.3712 ,  1.39 ,  1.4088 ,  1.4276 ,  1.4463 ,  1.4651 ,  1.4839 ,  1.5027 ,
	1.5215 ,  1.5403 ,  1.559 ,  1.5778 ,  1.5966 ,  1.6154 ,  1.6342 ,  1.653 ,  1.6718 ,  1.6905 ,
	1.7093 ,  1.7281 ,  1.7469 ,  1.7657 ,  1.7845 ,  1.8032 ,  1.822 ,  1.8408 ,  1.8596 ,  1.8784 ,
	1.8972 ,  1.9159 ,  1.9347 ,  1.9535 ,  1.9723 ,  1.9911 ,  2.0099 ,  2.0286 ,  2.0474 ,  2.0662 ,
	2.085 ,  2.1038 ,  2.1226 ,  2.1413 ,  2.1601 ,  2.1789 ,  2.1977 ,  2.2165 ,  2.2353 ,  2.254 ,
	2.2728 ,  2.2916 ,  2.3104 ,  2.3292 ,  2.348 ,  2.3668 ,  2.3855 ,  2.4043 ,  2.4231 ,  2.4419 ,
	2.4607 ,  2.4795 ,  2.4982 ,  2.517 ,  2.5358 ,  2.5546 ,  2.5734 ,  2.5922 ,  2.6109 ,  2.6297 ,
	2.6485 ,  2.6673 ,  2.6861 ,  2.7049 ,  2.7236 ,  2.7424 ,  2.7612 ,  2.78 ,  2.7988 ,  2.8176 ,
	2.8363 ,  2.8551 ,  2.8739 ,  2.8927 ,  2.9115 ,  2.9303 ,  2.949 ,  2.9678 ,  2.9866 ,  3.0054 ,
	3.0242 ,  3.043 ,  3.0617 ,  3.0805 ,  3.0993 ,  3.1181 ,  3.1369 ,  3.1557 ,  3.1745 ,  3.1932 ,
	3.212 ,  3.2308 ,  3.2496 ,  3.2684 ,  3.2872 ,  3.3059 ,  3.3247 ,  3.3435 ,  3.3623 ,  3.3811 ,
	3.3999 ,  3.4186 ,  3.4374 ,  3.4562 ,  3.475 ,  3.4938 ,  3.5126 ,  3.5313 ,  3.5501 ,  3.5689 ,
	3.5877 ,  3.6065 ,  3.6253 ,  3.644 ,  3.6628 ,  3.6816 ,  3.7004 ,  3.7192 ,  3.738 ,  3.7567 ,
	3.7755 ,  3.7943 ,  3.8131 ,  3.8319 ,  3.8507 ,  3.8694 ,  3.8882 ,  3.907 ,  3.9258 ,  3.9446 ,
	3.9634 ,  3.9822 ,  4.0009 ,  4.0197 ,  4.0385 ,  4.0573 ,  4.0761 ,  4.0949 ,  4.1136 ,  4.1324 ,
	4.1512 ,  4.17 ,  4.1888 ,  4.2076 ,  4.2263 ,  4.2451 ,  4.2639 ,  4.2827 ,  4.3015 ,  4.3203 ,
	4.339 ,  4.3578 ,  4.3766 ,  4.3954 ,  4.4142 ,  4.433 ,  4.4517 ,  4.4705 ,  4.4893 ,  4.5081 ,
	4.5269 ,  4.5457 ,  4.5644 ,  4.5832 ,  4.602 ,  4.6208 ,  4.6396 ,  4.6584 ,  4.6771 ,  4.6959 ,
	4.7147 ,  4.7335 ,  4.7523 ,  4.7711 ,  4.7899 ,  4.8086 ,  4.8274 ,  4.8462 ,  4.865 ,  4.8838 ,
	4.9026 ,  4.9213 ,  4.9401 ,  4.9589 ,  4.9777 ,  4.9965 ,  5.0153 ,  5.034 ,  5.0528 ,  5.0716 ,
	5.0904 ,  5.1092 ,  5.128 ,  5.1467 ,  5.1655 ,  5.1843 ,  5.2031 ,  5.2219 ,  5.2407 ,  5.2594 ,
	5.2782 ,  5.297 ,  5.3158 ,  5.3346 ,  5.3534 ,  5.3721 ,  5.3909 ,  5.4097 ,  5.4285 ,  5.4473 ,
	5.4661 ,  5.4848 ,  5.5036 ,  5.5224 ,  5.5412 ,  5.56 ,  5.5788 ,  5.5976 ,  5.6163 ,  5.6351 ,
	5.6539 ,  5.6727 ,  5.6915 ,  5.7103 ,  5.729 ,  5.7478 ,  5.7666 ,  5.7854 ,  5.8042 ,  5.823 ,
	5.8417 ,  5.8605 ,  5.8793 ,  5.8981 ,  5.9169 ,  5.9357 ,  5.9544 ,  5.9732 ,  5.992 ,  6.0108 ,
	6.0296 ,  6.0484 ,  6.0671 ,  6.0859 ,  6.1047 ,  6.1235 ,  6.1423 ,  6.1611 ,  6.1798 ,  6.1986 ,
	6.2174 ,  6.2362 ,  6.255 ,  6.2738 ,  6.2926 ,  6.3113 ,  6.3301 ,  6.3489 ,  6.3677 ,  6.3865 ,
	6.4053 ,  6.424 ,  6.4428 ,  6.4616 ,  6.4804 ,  6.4992 ,  6.518 ,  6.5367 ,  6.5555 ,  6.5743 ,
	6.5931 ,  6.6119 ,  6.6307 ,  6.6494 ,  6.6682 ,  6.687 ,  6.7058 ,  6.7246 ,  6.7434 ,  6.7621 ,
	6.7809 ,  6.7997 ,  6.8185 ,  6.8373 ,  6.8561 ,  6.8748 ,  6.8936 ,  6.9124 ,  6.9312 ,  6.95 ,
	6.9688 ,  6.9875 ,  7.0063 ,  7.0251 ,  7.0439 ,  7.0627 ,  7.0815 ,  7.1003 ,  7.119 ,  7.1378 ,
	7.1566 ,  7.1754 ,  7.1942 ,  7.213 ,  7.2317 ,  7.2505 ,  7.2693 ,  7.2881 ,  7.3069 ,  7.3257 ,
	7.3444 ,  7.3632 ,  7.382 ,  7.4008 ,  7.4196 ,  7.4384 ,  7.4571 ,  7.4759 ,  7.4947 ,  7.5135 ,
	7.5323 ,  7.5511 ,  7.5698 ,  7.5886 ,  7.6074 ,  7.6262 ,  7.645 ,  7.6638 ,  7.6825 ,  7.7013 ,
	7.7201 ,  7.7389 ,  7.7577 ,  7.7765 ,  7.7952 ,  7.814 ,  7.8328 ,  7.8516 ,  7.8704 ,  7.8892 ,
	7.908 ,  7.9267 ,  7.9455 ,  7.9643 ,  7.9831 ,  8.0019 ,  8.0207 ,  8.0394 ,  8.0582 ,  8.077 ,
	8.0958 ,  8.1146 ,  8.1334 ,  8.1521 ,  8.1709 ,  8.1897 ,  8.2085 ,  8.2273 ,  8.2461 ,  8.2648 ,
	8.2836 ,  8.3024 ,  8.3212 ,  8.34 ,  8.3588 ,  8.3775 ,  8.3963 ,  8.4151 ,  8.4339 ,  8.4527 ,
	8.4715 ,  8.4902 ,  8.509 ,  8.5278 ,  8.5466 ,  8.5654 ,  8.5842 ,  8.6029 ,  8.6217 ,  8.6405 ,
	8.6593 ,  8.6781 ,  8.6969 ,  8.7157 ,  8.7344 ,  8.7532 ,  8.772 ,  8.7908 ,  8.8096 ,  8.8284 ,
	8.8471 ,  8.8659 ,  8.8847 ,  8.9035 ,  8.9223 ,  8.9411 ,  8.9598 ,  8.9786 ,  8.9974 ,  9.0162 ,
	9.035 ,  9.0538 ,  9.0725 ,  9.0913 ,  9.1101 ,  9.1289 ,  9.1477 ,  9.1665 ,  9.1852 ,  9.204 ,
	9.2228 ,  9.2416 ,  9.2604 ,  9.2792 ,  9.2979 ,  9.3167 ,  9.3355 ,  9.3543 ,  9.3731 ,  9.3919 ,
	9.4106 ,  9.4294 ,  9.4482 ,  9.467 ,  9.4858 ,  9.5046 ,  9.5234 ,  9.5421 ,  9.5609 ,  9.5797 ,
	9.5985 ,  9.6173 ,  9.6361 ,  9.6548 ,  9.6736 ,  9.6924 ,  9.7112 ,  9.73 ,  9.7488 ,  9.7675 ,
	9.7863 ,  9.8051 ,  9.8239 ,  9.8427 ,  9.8615 ,  9.8802 ,  9.899 ,  9.9178 ,  9.9366 ,  9.9554 ,
	9.9742 ,  9.9929 ,  10.012 ,  10.031 ,  10.049 ,  10.068 ,  10.087 ,  10.106 ,  10.124 ,  10.143 ,
	10.162 ,  10.181 ,  10.2 ,  10.218 ,  10.237 ,  10.256 ,  10.275 ,  10.293 ,  10.312 ,  10.331 ,
	10.35 ,  10.369 ,  10.387 ,  10.406 ,  10.425 ,  10.444 ,  10.463 ,  10.481 ,  10.5 ,  10.519 ,
	10.538 ,  10.556 ,  10.575 ,  10.594 ,  10.613 ,  10.632 ,  10.65 ,  10.669 ,  10.688 ,  10.707 ,
	10.726 ,  10.744 ,  10.763 ,  10.782 ,  10.801 ,  10.819 ,  10.838 ,  10.857 ,  10.876 ,  10.895 ,
	10.913 ,  10.932 ,  10.951 ,  10.97 ,  10.988 ,  11.007 ,  11.026 ,  11.045 ,  11.064 ,  11.082 ,
	11.101 ,  11.12 ,  11.139 ,  11.158 ,  11.176 ,  11.195 ,  11.214 ,  11.233 ,  11.251 ,  11.27 ,
	11.289 ,  11.308 ,  11.327 ,  11.345 ,  11.364 ,  11.383 ,  11.402 ,  11.421 ,  11.439 ,  11.458 ,
	11.477 ,  11.496 ,  11.514 ,  11.533 ,  11.552 ,  11.571 ,  11.59 ,  11.608 ,  11.627 ,  11.646 ,
	11.665 ,  11.683 ,  11.702 ,  11.721 ,  11.74 ,  11.759 ,  11.777 ,  11.796 ,  11.815 ,  11.834 ,
	11.853 ,  11.871 ,  11.89 ,  11.909 ,  11.928 ,  11.946 ,  11.965 ,  11.984 ,  12.003 ,  12.022 ,
	12.04 ,  12.059 ,  12.078 ,  12.097 ,  12.116 ,  12.134 ,  12.153 ,  12.172 ,  12.191 ,  12.209 ,
	12.228 ,  12.247 ,  12.266 ,  12.285 ,  12.303 ,  12.322 ,  12.341 ,  12.36 ,  12.378 ,  12.397 ,
	12.416 ,  12.435 ,  12.454 ,  12.472 ,  12.491 ,  12.51 ,  12.529 ,  12.548 ,  12.566 ,  12.585 ,
	12.604 ,  12.623 ,  12.641 ,  12.66 ,  12.679 ,  12.698 ,  12.717 ,  12.735 ,  12.754 ,  12.773 ,
	12.792 ,  12.811 ,  12.829 ,  12.848 ,  12.867 ,  12.886 ,  12.904 ,  12.923 ,  12.942 ,  12.961 ,
	12.98 ,  12.998 ,  13.017 ,  13.036 ,  13.055 ,  13.073 ,  13.092 ,  13.111 ,  13.13 ,  13.149 ,
	13.167 ,  13.186 ,  13.205 ,  13.224 ,  13.243 ,  13.261 ,  13.28 ,  13.299 ,  13.318 ,  13.336 ,
	13.355 ,  13.374 ,  13.393 ,  13.412 ,  13.43 ,  13.449 ,  13.468 ,  13.487 ,  13.506 ,  13.524 ,
	13.543 ,  13.562 ,  13.581 ,  13.599 ,  13.618 ,  13.637 ,  13.656 ,  13.675 ,  13.693 ,  13.712 ,
	13.731 ,  13.75 ,  13.768 ,  13.787 ,  13.806 ,  13.825 ,  13.844 ,  13.862 ,  13.881 ,  13.9 ,
	13.919 ,  13.938 ,  13.956 ,  13.975 ,  13.994 ,  14.013 ,  14.031 ,  14.05 ,  14.069 ,  14.088 ,
	14.107 ,  14.125 ,  14.144 ,  14.163 ,  14.182 ,  14.201 ,  14.219 ,  14.238 ,  14.257 ,  14.276 ,
	14.294 ,  14.313 ,  14.332 ,  14.351 ,  14.37 ,  14.388 ,  14.407 ,  14.426 ,  14.445 ,  14.463 ,
	14.482 ,  14.501 ,  14.52 ,  14.539 ,  14.557 ,  14.576 ,  14.595 ,  14.614 ,  14.633 ,  14.651 ,
	14.67 ,  14.689 ,  14.708 ,  14.726 ,  14.745 ,  14.764 ,  14.783 ,  14.802 ,  14.82 ,  14.839 ,
	14.858 ,  14.877 ,  14.895 ,  14.914 ,  14.933 ,  14.952 ,  14.971 ,  14.989 ,  15.008 ,  15.027 ,
	15.046 ,  15.065 ,  15.083 ,  15.102 ,  15.121 ,  15.14 ,  15.158 ,  15.177 ,  15.196 ,  15.215 ,
	15.234 ,  15.252 ,  15.271 ,  15.29 ,  15.309 ,  15.328 ,  15.346 ,  15.365 ,  15.384 ,  15.403 ,
	15.421 ,  15.44 ,  15.459 ,  15.478 ,  15.497 ,  15.515 ,  15.534 ,  15.553 ,  15.572 ,  15.59 ,
	15.609 ,  15.628 ,  15.647 ,  15.666 ,  15.684 ,  15.703 ,  15.722 ,  15.741 ,  15.76 ,  15.778 ,
	15.797 ,  15.816 ,  15.835 ,  15.853 ,  15.872 ,  15.891 ,  15.91 ,  15.929 ,  15.947 ,  15.966 ,
	15.985 ,  16.004 ,  16.023 ,  16.041 ,  16.06 ,  16.079 ,  16.098 ,  16.116 ,  16.135 ,  16.154 ,
	16.173 ,  16.192 ,  16.21 ,  16.229 ,  16.248 ,  16.267 ,  16.285 ,  16.304 ,  16.323 ,  16.342 ,
	16.361 ,  16.379 ,  16.398 ,  16.417 ,  16.436 ,  16.455 ,  16.473 ,  16.492 ,  16.511 ,  16.53 ,
	16.548 ,  16.567 ,  16.586 ,  16.605 ,  16.624 ,  16.642 ,  16.661 ,  16.68 ,  16.699 ,  16.718 ,
	16.736 ,  16.755 ,  16.774 ,  16.793 ,  16.811 ,  16.83 ,  16.849 ,  16.868 ,  16.887 ,  16.905 ,
	16.924 ,  16.943 ,  16.962 ,  16.98 ,  16.999 ,  17.018 ,  17.037 ,  17.056 ,  17.074 ,  17.093 ,
	17.112 ,  17.131 ,  17.15 ,  17.168 ,  17.187 ,  17.206 ,  17.225 ,  17.243 ,  17.262 ,  17.281 ,
	17.3 ,  17.319 ,  17.337 ,  17.356 ,  17.375 ,  17.394 ,  17.413 ,  17.431 ,  17.45 ,  17.469 ,
	17.488 ,  17.506 ,  17.525 ,  17.544 ,  17.563 ,  17.582 ,  17.6 ,  17.619 ,  17.638 ,  17.657 ,
	17.675 ,  17.694 ,  17.713 ,  17.732 ,  17.751 ,  17.769 ,  17.788 ,  17.807 ,  17.826 ,  17.845 ,
	17.863 ,  17.882 ,  17.901 ,  17.92 ,  17.938 ,  17.957 ,  17.976 ,  17.995 ,  18.014 ,  18.032 ,
	18.051 ,  18.07 ,  18.089 ,  18.108 ,  18.126 ,  18.145 ,  18.164 ,  18.183 ,  18.201 ,  18.22 ,
	18.239 ,  18.258 ,  18.277 ,  18.295 ,  18.314 ,  18.333 ,  18.352 ,  18.37 ,  18.389 ,  18.408 ,
	18.427 ,  18.446 ,  18.464 ,  18.483 ,  18.502 ,  18.521 ,  18.54 ,  18.558 ,  18.577 ,  18.596 ,
	18.615 ,  18.633 ,  18.652 ,  18.671 ,  18.69 ,  18.709 ,  18.727 ,  18.746 ,  18.765 ,  18.784
],

weights: [
	0.00826168 ,  0.03298232 ,  0.0740656 ,  0.1314152 ,  0.204932 ,  0.2945264 ,  0.4000976 ,  0.5215559999999999 ,  0.658784 ,  0.81172 ,
	0.98028 ,  1.164296 ,  1.3637679999999999 ,  1.578528 ,  1.808464 ,  2.053576 ,  2.3136959999999998 ,  2.588824 ,  2.878736 ,  3.183376 ,
	3.502744 ,  3.8366160000000002 ,  4.184992 ,  4.5477039999999995 ,  4.924752 ,  5.3160240000000005 ,  5.72152 ,  6.14096 ,  6.57384 ,  7.02128 ,
	7.4821599999999995 ,  7.956479999999999 ,  8.44424 ,  8.946 ,  9.46064 ,  9.988719999999999 ,  10.530240000000001 ,  11.08464 ,  11.652479999999999 ,  12.23264 ,
	12.82624 ,  13.43216 ,  14.0504 ,  14.68208 ,  15.32552 ,  15.98184 ,  16.649919999999998 ,  17.33088 ,  18.023600000000002 ,  18.72808 ,
	19.444879999999998 ,  20.17344 ,  20.91376 ,  21.6664 ,  22.429679999999998 ,  23.205280000000002 ,  23.99152 ,  24.78952 ,  25.59872 ,  26.41912 ,
	27.25072 ,  28.093519999999998 ,  28.94696 ,  29.81104 ,  30.68632 ,  31.57168 ,  32.468239999999994 ,  33.374880000000005 ,  34.29216 ,  35.219519999999996 ,
	36.157520000000005 ,  37.1056 ,  38.063759999999995 ,  39.03144 ,  40.0092 ,  40.99704 ,  41.99496 ,  43.00184 ,  44.0188 ,  45.04472 ,
	46.08072 ,  47.12568 ,  48.1796 ,  49.24304 ,  50.315999999999995 ,  51.39736 ,  52.48768 ,  53.58696 ,  54.6952 ,  55.812400000000004 ,
	56.9352 ,  58.072 ,  59.2144 ,  60.3624 ,  61.5216 ,  62.692 ,  63.86800000000001 ,  65.0496 ,  66.24239999999999 ,  67.44080000000001 ,
	68.6504 ,  69.8656 ,  71.0864 ,  72.3184 ,  73.556 ,  74.7992 ,  76.0536 ,  77.3136 ,  78.5792 ,  79.85040000000001 ,
	81.1328 ,  82.4208 ,  83.71440000000001 ,  85.0192 ,  86.3296 ,  87.64 ,  88.9616 ,  90.2944 ,  91.6272 ,  92.9712 ,
	94.31519999999999 ,  95.6704 ,  97.0312 ,  98.3976 ,  99.7696 ,  101.1472 ,  102.5304 ,  103.9192 ,  105.3136 ,  106.7136 ,
	108.1248 ,  109.536 ,  110.95280000000001 ,  112.37519999999999 ,  113.80319999999999 ,  115.23679999999999 ,  116.676 ,  118.1208 ,  119.5656 ,  121.02159999999999 ,
	122.4776 ,  123.94479999999999 ,  125.412 ,  126.88479999999998 ,  128.3576 ,  129.8416 ,  131.3256 ,  132.8208 ,  134.31040000000002 ,  135.81119999999999 ,
	137.3176 ,  138.824 ,  140.33599999999998 ,  141.848 ,  143.3656 ,  144.8888 ,  146.4176 ,  147.9464 ,  149.4808 ,  151.0208 ,
	152.5608 ,  154.10639999999998 ,  155.652 ,  157.2032 ,  158.76 ,  160.3168 ,  161.8736 ,  163.4416 ,  165.00400000000002 ,  166.5776 ,
	168.1456 ,  169.72480000000002 ,  171.29840000000002 ,  172.8832 ,  174.4624 ,  176.0528 ,  177.63760000000002 ,  179.228 ,  180.8184 ,  182.4144 ,
	184.0104 ,  185.61200000000002 ,  187.21359999999999 ,  188.81519999999998 ,  190.42239999999998 ,  192.0296 ,  193.6368 ,  195.24960000000002 ,  196.86239999999998 ,  198.4752 ,
	200.0936 ,  201.7064 ,  203.3248 ,  204.9488 ,  206.56719999999999 ,  208.1912 ,  209.8152 ,  211.4392 ,  213.0632 ,  214.6928 ,
	216.3224 ,  217.952 ,  219.5816 ,  221.21120000000002 ,  222.8408 ,  224.476 ,  226.1056 ,  227.7408 ,  229.376 ,  231.0056 ,
	232.64079999999998 ,  234.276 ,  235.9112 ,  237.5464 ,  239.1816 ,  240.8224 ,  242.4576 ,  244.0928 ,  245.728 ,  247.36320000000003 ,
	248.99839999999998 ,  250.63359999999997 ,  252.2688 ,  253.904 ,  255.5392 ,  257.1688 ,  258.80400000000003 ,  260.4392 ,  262.0688 ,  263.6984 ,
	265.3336 ,  266.96320000000003 ,  268.5928 ,  270.2168 ,  271.84639999999996 ,  273.47040000000004 ,  275.1 ,  276.724 ,  278.348 ,  279.9664 ,
	281.5904 ,  283.2088 ,  284.8272 ,  286.4456 ,  288.0584 ,  289.6712 ,  291.284 ,  292.8968 ,  294.504 ,  296.1112 ,
	297.71840000000003 ,  299.32 ,  300.9216 ,  302.5232 ,  304.11920000000003 ,  305.7152 ,  307.3112 ,  308.9016 ,  310.492 ,  312.0824 ,
	313.66720000000004 ,  315.252 ,  316.8312 ,  318.41040000000004 ,  319.984 ,  321.55760000000004 ,  323.13120000000004 ,  324.69919999999996 ,  326.2672 ,  327.8296 ,
	329.3864 ,  330.9488 ,  332.5 ,  334.0568 ,  335.6024 ,  337.148 ,  338.6936 ,  340.23359999999997 ,  341.7736 ,  343.308 ,
	344.8368 ,  346.3656 ,  347.8888 ,  349.41200000000003 ,  350.9296 ,  352.4416 ,  353.9536 ,  355.4656 ,  356.96639999999996 ,  358.46720000000005 ,
	359.968 ,  361.4576 ,  362.94719999999995 ,  364.4368 ,  365.9208 ,  367.3992 ,  368.872 ,  370.34479999999996 ,  371.812 ,  373.27360000000004 ,
	374.7352 ,  376.1856 ,  377.6416 ,  379.0864 ,  380.5312 ,  381.9704 ,  383.404 ,  384.832 ,  386.26 ,  387.68240000000003 ,
	389.0992 ,  390.51039999999995 ,  391.916 ,  393.3216 ,  394.72159999999997 ,  396.116 ,  397.50480000000005 ,  398.8936 ,  400.2712 ,  401.6488 ,
	403.02079999999995 ,  404.3872 ,  405.748 ,  407.10319999999996 ,  408.45840000000004 ,  409.8024 ,  411.1464 ,  412.4848 ,  413.812 ,  415.13919999999996 ,
	416.4608 ,  417.77680000000004 ,  419.09279999999995 ,  420.3976 ,  421.6968 ,  422.996 ,  424.284 ,  425.56640000000004 ,  426.84880000000004 ,  428.12 ,
	429.3912 ,  430.65680000000003 ,  431.9112 ,  433.1656 ,  434.4144 ,  435.65200000000004 ,  436.88960000000003 ,  438.116 ,  439.3424 ,  440.5576 ,
	441.7728 ,  442.97679999999997 ,  444.18080000000003 ,  445.37359999999995 ,  446.5664 ,  447.748 ,  448.92400000000004 ,  450.09439999999995 ,  451.25919999999996 ,  452.41839999999996 ,
	453.572 ,  454.71999999999997 ,  455.8624 ,  456.9992 ,  458.12480000000005 ,  459.2504 ,  460.3648 ,  461.4736 ,  462.57680000000005 ,  463.6744 ,
	464.7664 ,  465.8528 ,  466.92800000000005 ,  468.0032 ,  469.0672 ,  470.12559999999996 ,  471.1784 ,  472.2256 ,  473.2616 ,  474.2976 ,
	475.3224 ,  476.3416 ,  477.3552 ,  478.3576 ,  479.36 ,  480.3512 ,  481.33680000000004 ,  482.3168 ,  483.2912 ,  484.25440000000003 ,
	485.212 ,  486.164 ,  487.11039999999997 ,  488.0456 ,  488.98080000000004 ,  489.9048 ,  490.8232 ,  491.73040000000003 ,  492.632 ,  493.528 ,
	494.41839999999996 ,  495.30319999999995 ,  496.1768 ,  497.0448 ,  497.90160000000003 ,  498.7584 ,  499.6039999999999 ,  500.44399999999996 ,  501.2728 ,  502.09600000000006 ,
	502.9136 ,  503.7256 ,  504.52639999999997 ,  505.3216 ,  506.1112 ,  506.88960000000003 ,  507.6624 ,  508.4296 ,  509.18559999999997 ,  509.936 ,
	510.6808 ,  511.41999999999996 ,  512.148 ,  512.8648 ,  513.5816 ,  514.2872 ,  514.9816 ,  515.6759999999999 ,  516.3592000000001 ,  517.0312 ,
	517.7031999999999 ,  518.364 ,  519.0136 ,  519.6576 ,  520.2959999999999 ,  520.9232000000001 ,  521.5448 ,  522.1608 ,  522.7656 ,  523.3648 ,
	523.9584 ,  524.5408 ,  525.112 ,  525.6831999999999 ,  526.2432 ,  526.792 ,  527.3352000000001 ,  527.8728 ,  528.3992 ,  528.92 ,
	529.4295999999999 ,  529.9391999999999 ,  530.4319999999999 ,  530.9192 ,  531.4008 ,  531.8768 ,  532.336 ,  532.7952 ,  533.2432 ,  533.6856 ,
	534.1168 ,  534.5423999999999 ,  534.9568 ,  535.3656 ,  535.7632000000001 ,  536.1552 ,  536.5415999999999 ,  536.9168 ,  537.2864000000001 ,  537.6448 ,
	537.9976 ,  538.3392 ,  538.6752 ,  539.0056000000001 ,  539.3248000000001 ,  539.6328 ,  539.9408 ,  540.232 ,  540.5176 ,  540.7976 ,
	541.0664 ,  541.3296 ,  541.5816 ,  541.828 ,  542.0688 ,  542.2984 ,  542.5168 ,  542.7296 ,  542.9368 ,  543.1328000000001 ,
	543.3176 ,  543.5024 ,  543.6704000000001 ,  543.8328 ,  543.9896 ,  544.1352 ,  544.2751999999999 ,  544.404 ,  544.5272 ,  544.6448 ,
	544.7456 ,  544.8464 ,  544.936 ,  545.0144 ,  545.0872 ,  545.1544 ,  545.2104 ,  545.2552 ,  545.2944 ,  545.328 ,
	545.3504 ,  545.3616000000001 ,  545.3672 ,  545.3672 ,  545.356 ,  545.3392 ,  545.3112 ,  545.2776 ,  545.2328 ,  545.1824 ,
	545.1208 ,  545.0536 ,  544.9752 ,  544.8912 ,  544.7959999999999 ,  544.6952 ,  544.5888 ,  544.4712 ,  544.3424 ,  544.208 ,
	544.0624 ,  543.9168000000001 ,  543.7544 ,  543.5864 ,  543.4128000000001 ,  543.228 ,  543.0376 ,  542.836 ,  542.6232 ,  542.4104 ,
	542.1808 ,  541.9512 ,  541.7103999999999 ,  541.4584 ,  541.2008 ,  540.932 ,  540.6576 ,  540.3775999999999 ,  540.0864 ,  539.7896000000001 ,
	539.4816 ,  539.1623999999999 ,  538.8431999999999 ,  538.5072 ,  538.1712 ,  537.824 ,  537.4656 ,  537.1016 ,  536.7320000000001 ,  536.3512 ,
	535.9592 ,  535.5672 ,  535.1584 ,  534.7496 ,  534.3296 ,  533.8984 ,  533.4616000000001 ,  533.0192 ,  532.5656 ,  532.1064 ,
	531.6360000000001 ,  531.1600000000001 ,  530.6727999999999 ,  530.18 ,  529.6816 ,  529.1719999999999 ,  528.6568 ,  528.1303999999999 ,  527.5984000000001 ,  527.0608000000001 ,
	526.5120000000001 ,  525.9576 ,  525.3919999999999 ,  524.8208 ,  524.244 ,  523.656 ,  523.0568 ,  522.4576000000001 ,  521.8472 ,  521.2256 ,
	520.604 ,  519.9656 ,  519.3272000000001 ,  518.6776 ,  518.0224 ,  517.356 ,  516.684 ,  516.0064 ,  515.3176000000001 ,  514.6232 ,
	513.9176 ,  513.212 ,  512.4896 ,  511.76719999999995 ,  511.03360000000004 ,  510.29440000000005 ,  509.54400000000004 ,  508.79359999999997 ,  508.02639999999997 ,  507.25919999999996 ,
	506.4808 ,  505.6968 ,  504.9016 ,  504.10639999999995 ,  503.3 ,  502.4824 ,  501.65919999999994 ,  500.83599999999996 ,  499.9959999999999 ,  499.15599999999995 ,
	498.30480000000006 ,  497.44800000000004 ,  496.58000000000004 ,  495.7064 ,  494.82720000000006 ,  493.94239999999996 ,  493.04640000000006 ,  492.15040000000005 ,  491.2432 ,  490.32480000000004 ,
	489.40639999999996 ,  488.4768 ,  487.5416 ,  486.6008 ,  485.6488 ,  484.6912 ,  483.728 ,  482.75919999999996 ,  481.7848 ,  480.7992 ,
	479.80800000000005 ,  478.8112 ,  477.80879999999996 ,  476.79519999999997 ,  475.776 ,  474.7568 ,  473.7208 ,  472.6848 ,  471.64320000000004 ,  470.5904 ,
	469.5376 ,  468.4736 ,  467.3984 ,  466.3232 ,  465.24240000000003 ,  464.1504 ,  463.0584 ,  461.9552 ,  460.8464 ,  459.732 ,
	458.612 ,  457.4864 ,  456.34959999999995 ,  455.21279999999996 ,  454.0648 ,  452.91119999999995 ,  451.75759999999997 ,  450.5928 ,  449.42240000000004 ,  448.2464 ,
	447.0648 ,  445.87760000000003 ,  444.6792 ,  443.4808 ,  442.27680000000004 ,  441.0616 ,  439.8464 ,  438.62559999999996 ,  437.3936 ,  436.1616 ,
	434.91839999999996 ,  433.6752 ,  432.4208 ,  431.1664 ,  429.9008 ,  428.6352 ,  427.3584 ,  426.08160000000004 ,  424.7936 ,  423.5056 ,
	422.212 ,  420.9128 ,  419.6024 ,  418.29200000000003 ,  416.976 ,  415.65439999999995 ,  414.3272 ,  412.9944 ,  411.6616 ,  410.31759999999997 ,
	408.968 ,  407.61839999999995 ,  406.2632 ,  404.89680000000004 ,  403.5304 ,  402.1584 ,  400.7864 ,  399.4032 ,  398.02000000000004 ,  396.6256 ,
	395.2312 ,  393.83119999999997 ,  392.4256 ,  391.02 ,  389.6032 ,  388.1864 ,  386.764 ,  385.336 ,  383.908 ,  382.4688 ,
	381.0296 ,  379.5848 ,  378.14 ,  376.684 ,  375.228 ,  373.77200000000005 ,  372.3048 ,  370.8376 ,  369.3648 ,  367.88640000000004 ,
	366.40799999999996 ,  364.924 ,  363.43440000000004 ,  361.9448 ,  360.4496 ,  358.9488 ,  357.44800000000004 ,  355.9416 ,  354.4296 ,  352.9176 ,
	351.4 ,  349.8768 ,  348.35360000000003 ,  346.8304 ,  345.296 ,  343.7616 ,  342.2272 ,  340.6872 ,  339.1472 ,  337.596 ,
	336.05039999999997 ,  334.4992 ,  332.9424 ,  331.3856 ,  329.8232 ,  328.26079999999996 ,  326.69280000000003 ,  325.1248 ,  323.5512 ,  321.97760000000005 ,
	320.404 ,  318.81919999999997 ,  317.24 ,  315.6552 ,  314.0704 ,  312.48 ,  310.8896 ,  309.2936 ,  307.69759999999997 ,  306.1016 ,
	304.5 ,  302.8984 ,  301.2968 ,  299.6896 ,  298.0824 ,  296.46959999999996 ,  294.8568 ,  293.244 ,  291.6312 ,  290.01279999999997 ,
	288.3944 ,  286.77599999999995 ,  285.152 ,  283.53360000000004 ,  281.9096 ,  280.28 ,  278.656 ,  277.0264 ,  275.39680000000004 ,  273.7672 ,
	272.13759999999996 ,  270.5024 ,  268.8672 ,  267.232 ,  265.5968 ,  263.9616 ,  262.3264 ,  260.6856 ,  259.0504 ,  257.4096 ,
	255.76880000000003 ,  254.13360000000003 ,  252.4928 ,  250.84640000000002 ,  249.2056 ,  247.5648 ,  245.924 ,  244.2832 ,  242.6424 ,  240.99599999999998 ,
	239.3552 ,  237.7144 ,  236.068 ,  234.4272 ,  232.7864 ,  231.1456 ,  229.50480000000002 ,  227.864 ,  226.2232 ,  224.5824 ,
	222.9416 ,  221.3064 ,  219.6656 ,  218.0304 ,  216.3952 ,  214.7544 ,  213.1248 ,  211.4896 ,  209.8544 ,  208.2248 ,
	206.5952 ,  204.9656 ,  203.336 ,  201.7064 ,  200.0824 ,  198.45839999999998 ,  196.83440000000002 ,  195.216 ,  193.5976 ,  191.97920000000002 ,
	190.36079999999998 ,  188.748 ,  187.1352 ,  185.5224 ,  183.9152 ,  182.308 ,  180.7064 ,  179.1048 ,  177.50320000000002 ,  175.90720000000002 ,
	174.3112 ,  172.7208 ,  171.1304 ,  169.54000000000002 ,  167.9552 ,  166.376 ,  164.7968 ,  163.2176 ,  161.644 ,  160.0704 ,
	158.50240000000002 ,  156.94 ,  155.3776 ,  153.8208 ,  152.26399999999998 ,  150.71280000000002 ,  149.1616 ,  147.61599999999999 ,  146.076 ,  144.536 ,
	143.00719999999998 ,  141.47279999999998 ,  139.9496 ,  138.4264 ,  136.9032 ,  135.3912 ,  133.8792 ,  132.37279999999998 ,  130.872 ,  129.3712 ,
	127.87599999999999 ,  126.38640000000001 ,  124.9024 ,  123.42399999999999 ,  121.9456 ,  120.4784 ,  119.0112 ,  117.5496 ,  116.09360000000001 ,  114.63759999999999 ,
	113.19279999999999 ,  111.7536 ,  110.3144 ,  108.8864 ,  107.4584 ,  106.0416 ,  104.6248 ,  103.2136 ,  101.8136 ,  100.4136 ,
	99.0192 ,  97.636 ,  96.25280000000001 ,  94.88080000000001 ,  93.50880000000001 ,  92.148 ,  90.7928 ,  89.4432 ,  88.0992 ,  86.7608 ,
	85.4336 ,  84.1064 ,  82.7904 ,  81.48 ,  80.1752 ,  78.87599999999999 ,  77.58800000000001 ,  76.3056 ,  75.0288 ,  73.76320000000001 ,
	72.49759999999999 ,  71.2432 ,  70.0 ,  68.75680000000001 ,  67.5248 ,  66.30399999999999 ,  65.0832 ,  63.879200000000004 ,  62.675200000000004 ,  61.482400000000005 ,
	60.300799999999995 ,  59.1192 ,  57.9544 ,  56.7896 ,  55.639359999999996 ,  54.495839999999994 ,  53.36016 ,  52.23344 ,  51.115120000000005 ,  50.00576 ,
	48.9048 ,  47.812799999999996 ,  46.72976 ,  45.655680000000004 ,  44.590559999999996 ,  43.53496 ,  42.48832 ,  41.4512 ,  40.42304 ,  39.404959999999996 ,
	38.39584 ,  37.3968 ,  36.40672 ,  35.427279999999996 ,  34.4568 ,  33.49696 ,  32.546640000000004 ,  31.606400000000004 ,  30.676799999999997 ,  29.75672 ,
	28.847279999999998 ,  27.94792 ,  27.0592 ,  26.18112 ,  25.31312 ,  24.45576 ,  23.609599999999997 ,  22.773519999999998 ,  21.94808 ,  21.13384 ,
	20.33024 ,  19.53784 ,  18.75608 ,  17.98552 ,  17.2256 ,  16.47688 ,  15.739360000000001 ,  15.01248 ,  14.2968 ,  13.591759999999999 ,
	12.897920000000001 ,  12.21472 ,  11.541599999999999 ,  10.87968 ,  10.22784 ,  9.586640000000001 ,  8.9544 ,  8.33168 ,  7.71736 ,  7.11144 ,
	6.51224 ,  5.91808 ,  5.32756 ,  4.737544000000001 ,  4.14372 ,  3.538976 ,  2.9110479999999996 ,  2.234456 ,  1.441384 ,  0.0056
],

Emax: 18.8,
},

direction: "isotropic",

  log: [ { file: "stdout", level: "disabled" } ],

}
