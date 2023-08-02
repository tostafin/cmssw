#usage: ./getIovList.sh <db> <tag> <limit (default=2000)>
conddb --db $1 list $2 --limit ${3:-2000}  | awk '{print $3}'| rev | cut -c2- | rev | cut -c2- | tail -n +4