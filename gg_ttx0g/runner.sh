while getopts lhe:rwgt:out: flag
do
    case "${flag}" in
        lhe) lhePt=${OPTARG};;
        rwgt) rePt=${OPTARG};;
        out) outputPt=${OPTARG};;
    esac
done
./SubProcesses/P1_Sigma_sm_gg_ttx/runner.exe "-lhe=$lhePt -rwgt=$rePt -out=$outputPt"; 