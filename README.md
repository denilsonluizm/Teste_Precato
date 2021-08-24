# Teste_Precato
Para a solucao do desafio proponho 2 formas:

Primeira: em Teste_Precato_CPP_Denilson_luiz.cpp
Banco de dados relacional em tabelas .csv (CREDOR.csv, ENTE_DEVEDOR.csv e PAGAMENTO.csv), as três tabelas se relacionam pelas primary Keys id_credor, id_ente_devedor e id_remessa.
A função principal que cadastra os pagamentos é API_SolicitaPagamento() obedecendo as condições do problema.
Nesse formato de solução o código apresenta a implementação de funções para consulta e edição do banco de dados.

Segunda: em Teste_Precato_Python_e_SQL.ipynb
O banco de dados relacional é o mesmo das tabelas .csv do caso anterior, elas são simplesmente importadas para um banco de dados que chamamos de "Precato" em qualquer editor SQL; as tabelas se relacionam pelas mesmas primary Keys.
A única função implementada é SolicitaPagamento() que recebe como parâmetro uma class com os inputs.
Diferentemente do caso anterior não é preciso implementar funções para consulta ao banco de dados, pois utilizamos a biblioteca psycopg2 para consulta-lo utilizando SQL.
