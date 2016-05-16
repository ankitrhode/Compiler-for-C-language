testcases=`ls visible_tests/*.c`
for i in ${testcases[@]}; do
	./parser ${ARGS} < $i > junk
	subl $i
	subl junk
	read -p "Continue? [y/n] : " response
	if [ x${response} == "xn" ]; then
		break
	fi
done