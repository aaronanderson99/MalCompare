echo "DELETING ALL PREVIOUSLY DETECTED ATTACK PDFS"
rm results/FC_mimicry/* &> /dev/null
rm results/F_gdkde/* &> /dev/null
rm results/F_mimicry/* &> /dev/null
rm results/FTC_mimicry/* &> /dev/null
rm results/FT_gdkde/* &> /dev/null
rm results/FT_mimicry/* &> /dev/null
echo
echo "Finished deleting previously detected PDFs."
echo

for i in 1 2 3
do
	echo "FOLD $i - PREPARING FOR MODEL RETRAIN"
	rm data/F-scaled.model &> /dev/null
	rm data/FC.model &> /dev/null
	rm data/FT-scaled.model &> /dev/null
	rm data/FTC.model &> /dev/null
	rm data/surrogate-scaled.csv &> /dev/null
	rm data/surrogate.csv &> /dev/null
	rm data/contagio-scaled.csv &> /dev/null
	rm data/contagio.csv &> /dev/null
	cp "data/newCSVs/surrogate-scaled - $i.csv" "data/surrogate-scaled.csv"
	cp "data/newCSVs/surrogate - $i.csv" "data/surrogate.csv"
	cp "data/newCSVs/contagio-scaled - $i.csv" "data/contagio-scaled.csv"
	cp "data/newCSVs/contagio - $i.csv" "data/contagio.csv"
	echo "THE NEXT EXPERIMENT WILL RETRAIN MODELS"
	sleep 3

	# # SVM classifier, Surrogate (VirusTotal) dataset, Mimicry and GD-KDE attacks
	# echo "F - PREPARING FOLD #$i"
	# rm data/attack.list
	# cp "data/newCSVs/surrogate-scaled - attack$i.list" "data/attack.list"
	# echo "F - RUNNING FOLD #$i.py"
	# echo "Current time: $(date +"%T")"
	# #python reproduction/F.py | tee log_F.txt
	# python reproduction/F.py > "log_F_$i.txt"
	# echo
	# echo "Finished running F.py at $(date +"%T")"
	# echo

	# # Random Forest classifier, Surrogate (VirusTotal) dataset, Mimicry attack
	# echo "FC - PREPARING FOLD #$i"
	# rm data/attack.list
	# cp "data/newCSVs/surrogate - attack$i.list" "data/attack.list"
	# echo "FC - RUNNING FOLD #$i.py"
	# echo "Current time: $(date +"%T")"
	# python reproduction/FC.py > "log_FC_$i.txt"
	# echo
	# echo "Finished running FC.py at $(date +"%T")"
	# echo

	# SVM classifier, Contagio dataset, Mimicry and GD-KDE attacks
	echo "FT - PREPARING FOLD #$i"
	rm data/attack.list &> /dev/null
	cp "data/newCSVs/contagio-scaled - attack$i.list" "data/attack.list"
	echo "FT - RUNNING FOLD #$i.py"
	echo "Current time: $(date +"%T")"
	python reproduction/FT.py > "log_FT_$i.txt"
	echo
	echo "Finished running FT.py at $(date +"%T")"
	echo

	# Random Forest classifier, Contagio dataset, Mimicry attack
	echo "FTC - PREPARING FOLD #$i"
	rm data/attack.list &> /dev/null
	cp "data/newCSVs/contagio - attack$i.list" "data/attack.list"
	echo "FTC - RUNNING FOLD #$i.py"
	echo "Current time: $(date +"%T")"
	python reproduction/FTC.py > "log_FTC_$i.txt"
	echo
	echo "Finished running FTC.py at $(date +"%T")"
	echo
done