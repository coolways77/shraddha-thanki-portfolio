FROM gcc:12

WORKDIR /app

COPY . .

RUN g++ -o Main simulation/main.cpp simulation/simulation.cpp -Iinclude
RUN g++ -o Test simulation/test.cpp simulation/simulation.cpp -Iinclude
RUN g++ -o IntegrationTest simulation/integration_test.cpp simulation/simulation.cpp -Iinclude

CMD ["bash", "-c", "./Main && ./Test && ./IntegrationTest && mkdir -p output && cp *.csv *.png output/ && echo -e '\n=== Output Files ===' && ls -lh output"]

