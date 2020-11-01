for i in {1..100}; do echo -e "python alpha.py\n3.exe" | java -jar referee.jar >> out.txt; done
grep "Won\|Draw" out.txt | sed 's/[Player | Won]//g' > results.txt
python pieChart.py
start see.png