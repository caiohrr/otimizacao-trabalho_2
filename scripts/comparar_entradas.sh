#!/bin/bash

# Diretório onde os arquivos de entrada estão localizados
input_dir="../entradas_teste"

# Arquivo de saída para os logs
output_file="../log/saida_comissao.log"

# Lista de arquivos de entrada
input_files=(
        "entrada_teste_10-1.txt" "entrada_teste_10-2.txt" "entrada_teste_10-3.txt" "entrada_teste_10-4.txt"
        "entrada_teste_15-1.txt" "entrada_teste_15-2.txt" "entrada_teste_15-3.txt" "entrada_teste_15-4.txt"
        "entrada_teste_20-1.txt" "entrada_teste_20-2.txt" "entrada_teste_20-3.txt" "entrada_teste_20-4.txt"
        "entrada_teste_25-1.txt" "entrada_teste_25-2.txt" "entrada_teste_25-3.txt" "entrada_teste_25-4.txt"
        "entrada_teste_30-1.txt" "entrada_teste_30-2.txt" "entrada_teste_30-3.txt" "entrada_teste_30-4.txt"
)

# Limpa o arquivo de saída anterior, se existir
> $output_file

# Loop para rodar o executável para cada arquivo de entrada
for input_file in "${input_files[@]}"
do
        echo "Rodando para o arquivo: $input_dir/$input_file" >> $output_file
        .././comissao < "$input_dir/$input_file" &>> $output_file
        echo "-----------------------------------" >> $output_file
done

echo "Execução completa. Resultados armazenados em $output_file"
