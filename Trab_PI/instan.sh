
#leitura de arquivos

for i in 10 50 100 200 300 400
do
  for j in $(seq 10)
  do 
    ./out instances_kp/${i}_100_${j}.txt
  done
done
