while getopts lhe:rwgt:out: flag
do
    case "${flag}" in
        lhe) lhePt=${OPTARG};;
        rwgt) rePt=${OPTARG};;
        out) outputPt=${OPTARG};;
    esac
done
