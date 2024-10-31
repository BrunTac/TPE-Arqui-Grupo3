/*



void snake(){

    int cantPlayers = menu();


    drawMap();
    


}


int menu(){






}



void drawMap(){

	int width = sys_scrWidth();
    int height =  sys_scrHeight();

	for (size_t i = 0; i < width; i += 64)
	{
		for (size_t j = 0; j < height; j += 32)
		{
			if (j%64 == 0)
			{
				sys_drawSquare(DARK_GREEN, i, j);
				sys_drawSquare(GREEN, i+32, j);
			}else{
				sys_drawSquare(GREEN, i, j);
				sys_drawSquare(DARK_GREEN, i+32, j);
			}	
		}	
	}

	
}
*/