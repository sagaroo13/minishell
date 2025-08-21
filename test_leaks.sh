#!/bin/bash

# Colores para mejor visualización
GREEN='\033[0;32m'
RED='\033[0;31m'
YELLOW='\033[0;33m'
NC='\033[0m' # No Color

# Crear archivo temporal para los comandos
CMDS_FILE=$(mktemp)

# Escribe comandos de prueba en el archivo temporal
cat > "$CMDS_FILE" << 'EOF'
echo "Test básico"
ls | grep Makefile
cd ..
cd -
echo $?
export TEST=valor
echo $TEST
unset TEST
echo $TEST
cat << HERE
texto
multiples
lineas
HERE
exit
EOF

echo -e "${YELLOW}Ejecutando tests básicos con valgrind para detectar leaks...${NC}"
echo -e "${YELLOW}Ignorando leaks de readline...${NC}"

# Ejecutar valgrind con supresiones para readline
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --suppressions=./readline.supp \
         ./minishell < "$CMDS_FILE" 2> valgrind_output.txt

# Analizar el resultado
if grep -q "no leaks are possible" valgrind_output.txt; then
    echo -e "${GREEN}¡No se detectaron leaks en tus funciones!${NC}"
else
    echo -e "${RED}Se detectaron posibles leaks en tu código:${NC}"
    grep -A 5 "definitely lost:" valgrind_output.txt
    echo -e "${YELLOW}Ver valgrind_output.txt para más detalles${NC}"
fi

# Limpiar
rm "$CMDS_FILE"
echo -e "${GREEN}Pruebas completadas.${NC}"
