###############################################################################
#
#   Development commands for the project
#
#   AUTHOR: Maciek_Bak
#   CONTACT: wsciekly.maciek@gmail.com
#   CREATED: 04-08-2023
#
###############################################################################

.PHONY: help s0 s1 s2 s3 s4 s5 image container

help:
	@echo "help - display this message"
	@echo "s0 - render session 0 HTML (+ execute scripts)"
	@echo "s1 - render session 1 HTML (+ execute scripts)"
	@echo "s2 - render session 2 HTML (+ execute scripts)"
	@echo "s3 - render session 3 HTML (+ execute scripts)"
	@echo "s4 - render session 4 HTML (+ execute scripts)"
	@echo "s5 - render session 5 HTML (+ execute scripts)"
	@echo "image - build Docker image (requires Docker deamon running)"
	@echo "container - start interactive Docker container (requires image)"

s0:
	@jupyter nbconvert --to html sessions/0_SystemSetup/notebook.ipynb

s1:
	@jupyter nbconvert --to html sessions/1_CustomModels/notebook.ipynb

s2:
	@jupyter nbconvert --to html sessions/2_LinearAlgebra/notebook.ipynb

s3:
	@jupyter nbconvert --to html sessions/3_PythonAndCpp/notebook.ipynb

s4:
	@jupyter nbconvert --to html sessions/4_RAndCpp/notebook.ipynb

s5:
	@jupyter nbconvert --to html sessions/5_MachineLearning/notebook.ipynb

image:
	@# Build docker image
	@if [ $$(docker ps &> /dev/null; echo $$?) -eq 0 ]; then\
		docker rm efcomp &> /dev/null;\
		sleep 1;\
		docker rmi efcomp:latest &> /dev/null;\
		docker build -t efcomp:latest -f Dockerfile .;\
	else \
		echo "Docker deamon is not running OR the current user requires higher privileges.";\
	fi

container:
	@# Run a docker container
	@if [ $$((docker images | grep efcomp) &> /dev/null; echo $$?) -eq 0 ] \
	; then\
		docker rm efcomp &> /dev/null;\
		docker run --name efcomp -it efcomp:latest;\
	else \
		echo "Docker deamon isn't running OR efcomp image isn't available OR the current user requires higher privileges.";\
	fi
