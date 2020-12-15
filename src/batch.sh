
for i in {25..1000}
do
  echo "Processing $i..." >&2
  python3 manager.py $i 32
done
