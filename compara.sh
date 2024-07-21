#!/bin/bash

# Definir os nomes dos arquivos de saída
output_file_a="resultadosBoundDada.txt"
output_file_b="resultadosBoundNova.txt"
output_file_c="resultadosBruteForce.txt"

# Limpar os arquivos de saída se já existirem
> "$output_file_a"
> "$output_file_b"
> "$output_file_c"

# Loop através de todos os arquivos no diretório 'entradas'
for input_file in entradas/*; do
    # Verifica se é um arquivo regular
    if [[ -f "$input_file" ]]; then
        # Executar o programa 'comissao' com a flag -a
        ./comissao -a < "$input_file" >> $output_file_a
        
        # Executar o programa 'comissao' sem flag
        ./comissao < "$input_file" >> $output_file_b
        
        # Executar o programa 'comissao' com as flags -f -o flag
        ./comissao -f -o < "$input_file" >> $output_file_c
    fi
done

echo "Processamento concluído."
# Perguntar ao usuário se deseja executar um comando adicional
read -p "Deseja comparar no nvim? (y/n): " user_input

# Verificar a resposta do usuário
if [[ "$user_input" == "y" || "$user_input" == "Y" ]]; then
    # Comando adicional a ser executado
    nvim -O "$output_file_a" "$output_file_b" "$output_file_c"
fi
