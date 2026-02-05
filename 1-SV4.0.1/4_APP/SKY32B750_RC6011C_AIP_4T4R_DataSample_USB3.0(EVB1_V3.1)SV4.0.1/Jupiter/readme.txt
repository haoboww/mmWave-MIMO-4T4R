V3.9.1->V3.9.2:
OSPIO->CON1 |=(1<<OSPI_CON1_DIV_Pos) -> OSPIO->CON1 |=(4<<OSPI_CON1_DIV_Pos)解决跑一会采数乱的问题。