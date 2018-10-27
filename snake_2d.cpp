#include <allegro.h>
#include <cstdlib>
#include <ctime>
#include <sstream>

#define largura 1020
#define altura 600
#define limite_corpo 200
#define cima 0
#define baixo 1
#define esquerda 2
#define direita 3

BITMAP *buffer;	// Guarda todo o gráfico

// Carrega as imagens
BITMAP *logo;
BITMAP *cabecas;
BITMAP *cabeca;
BITMAP *corpo;
BITMAP *comida;

SAMPLE *som_menu;
SAMPLE *som_comer;
SAMPLE *som_gameover;
FONT *font1;

int limite_x = largura / 20;
int limite_y = altura / 20;
int sai = 0;
int tamanho = 3;
int direcao = direita;
int comida_x, comida_y;
int pontos = 0;
int velocidade = 100;
int gameover = 1;

struct corpo_estrutura
{
	int x, y;
};

corpo_estrutura corpo_v[limite_corpo];

void sair () {sai = 1;}

void setup ()
{
	allegro_init ();
	install_keyboard ();
	set_color_depth (32);
	install_sound (DIGI_AUTODETECT, MIDI_AUTODETECT, NULL);
	set_close_button_callback (sair);
	set_gfx_mode (GFX_AUTODETECT_WINDOWED, largura, altura, 0, 0);
    srand(time(NULL));

	buffer = create_bitmap (largura, altura);
	cabecas = create_bitmap (80, 20);
    cabeca = create_bitmap (20, 20);
	corpo = create_bitmap (20, 20);
	comida = create_bitmap (20, 20);

	logo = load_bitmap ("Sprites/menu.bmp", NULL);
	cabecas = load_bitmap ("Sprites/cabecas.bmp", NULL);
	corpo = load_bitmap ("Sprites/corpo.bmp", NULL);
	comida = load_bitmap ("Sprites/comida.bmp", NULL);

	som_menu = load_sample ("Outros/som_menu.wav");
	som_comer = load_sample("Outros/som_comer.wav");
	som_gameover = load_sample("Outros/som_gameover.wav");

	font1 = load_font ("Outros/alte.pcx", NULL, NULL);
}

void menu ()
{
    int time = 0;

    play_sample (som_menu, 255, 128, 1000, 1);

    while (!(sai || key[KEY_ESC]))
    {
        if (key[KEY_ENTER]) break;
        if (time > 100) time = 0;

        draw_sprite (buffer, logo, 0, 0);

        if (time < 50)

        textout_centre_ex (buffer, font, "PRESSIONE ENTER", largura/1.9, altura/1.5, 0x000000, -1);
        draw_sprite (screen, buffer, 0, 0);
        rest(10);
        clear(buffer);
        time ++;

    }
}

void desenhar_bordas_back ()
{
	//rectfill(BITMAP *, xo, yo, x1, y1, COLOR);
	// copia um retangulo preenchido com dois ponto em posiçoes opostas

	// SUPERIOR
	rectfill (buffer, 0, 40, largura, 60, 0x2F4F4F);

	// INFERIOR
	rectfill (buffer, 0, altura - 20, largura, altura, 0x2F4F4F);

	// ESQUERDO
	rectfill (buffer, 0, 40, 20, altura, 0x2F4F4F);

	// DIREITO
	rectfill (buffer, largura - 20 , 40, largura, altura, 0x2F4F4F);

	// BACKGROUND
    rectfill (buffer, 20, altura - 20, largura -20, 60, 0x66CDAA);
}

void update_tela ()
{
	blit(buffer, screen, 0, 0, 0, 0, largura, altura);
		// copia uma área retangular de um bitmap para outro

}

void gerar_snake ()
{
	// POSICAO CABECA
	corpo_v[0].x = 10;
	corpo_v[0].y = 9;

	for (int i=1; i <= tamanho; i++)
	{
		corpo_v[i].x = corpo_v[0].x - i;
		corpo_v[i].y = corpo_v[0].y;
	}
}

void gerar_comida ()
{
    comida_x = (rand() % (limite_x - 3)) + 2;
    // revisar
    comida_y = (rand() % (limite_y - 4)) + 3;

    int contador = 0;
    while (contador <= tamanho) // evita que a comida nasca no copro
    {
      if (comida_x == corpo_v[contador].x && comida_y == corpo_v[contador].y)
      {
         comida_x = (rand() % (limite_x - 3)) + 2;
         comida_y = (rand() % (limite_y - 4)) + 3;
         contador = 0;
      }

      else
      {
          contador ++;
      }

    }
}

void desenhar_comida ()
{
    draw_sprite (buffer, comida, comida_x*20, comida_y*20);
}

void desenhar_snake ()
{
	// DESENHAR CABECA
	switch(direcao)
	{
		case cima:
			blit(cabecas, cabeca, 0, 0, 0, 0, 20, 20);
			break;

		case baixo:
			blit(cabecas, cabeca, 20, 0, 0, 0, 20, 20);
			break;

		case esquerda:
			blit(cabecas, cabeca, 40, 0, 0, 0, 20, 20);
			break;

		case direita:
			blit(cabecas, cabeca, 60, 0, 0, 0, 20, 20);
			break;
	}

	draw_sprite(buffer, cabeca, corpo_v[0].x*20, corpo_v[0].y*20);

	// DESENHAR CORPO
	for (int i=1; i <= tamanho; i++)
	{
		draw_sprite(buffer, corpo, corpo_v[i].x*20, corpo_v[i].y*20);
	}
}

void update_direcao ()
{
	if (key[KEY_UP])
	{
	    if (direcao != baixo)
		{
		    direcao = cima;
		}
	}

	else if (key[KEY_DOWN])
	{
		if (direcao != cima)
		{
		    direcao = baixo;
		}
	}

	else if (key[KEY_RIGHT])
	{
		if (direcao != esquerda)
		{
		    direcao = direita;
		}
	}

	else if (key[KEY_LEFT])
	{
		if (direcao != direita)
		{
		    direcao = esquerda;
		}
	}
}

void game_over ()
{
    stop_sample(som_menu);
    play_sample (som_gameover, 255, 128, 1000, 0);

    clear(screen);
    textout_ex(screen, font, "GAME OVER", 480, 280, 0xFFFFFF, 0x000000);
    gameover = 0;
    rest(3000);
}

void mover_snake ()
{
    // UPDATE POSICAO CORPO
    for (int i=tamanho; i > 0; i--)
    {
        corpo_v[i] = corpo_v[i-1];
    }

    // UPDATE POSICAO CABECA
    switch (direcao)
    {
        case cima:
            corpo_v[0].y --;
            break;
        case baixo:
            corpo_v[0].y ++;
            break;
        case direita:
            corpo_v[0].x ++;
            break;
        case esquerda:
            corpo_v[0].x --;
            break;
    }

    // ROTINAS PARA COMER
    if ((corpo_v[0].x == comida_x) && (corpo_v[0].y == comida_y))
    {
       gerar_comida();
       tamanho ++;
       corpo_v[tamanho] = corpo_v[tamanho - 1];
       pontos = pontos + 30;
       play_sample (som_comer, 255, 128, 1000, 0);

       // AUMENTO DA VELOCIDADE
       if (pontos % 150 == 0 && pontos > 0 && velocidade > 40)
       {
           velocidade -= 10;
       }
    }

    // COLISAO COM BORDAS
    if (corpo_v[0].x < 1 || corpo_v[0].x > (limite_x - 2) || corpo_v[0].y < 3 || corpo_v[0].y > (limite_y - 2))
    {
        game_over ();
    }

    // COLISAO COM CORPO
    for (int i=1; i <= tamanho; i++)
    {
        if (corpo_v[0].x == corpo_v[i].x && corpo_v[0].y == corpo_v[i].y)
        {
            game_over();
            break;
        }
    }
}

void desenhar_pontos ()
{
    std::stringstream ss;
    ss << "PONTOS: " << pontos;
    std::string p = ss.str();

    char pontos_s[20];
    strcpy(pontos_s, p.c_str());
    textout_ex(buffer, font, pontos_s, 15, 15, 0xFFFFFF,  0x000000);


}

int main()
{
	setup ();

    while (gameover >= 0)
    {


        while (!key[KEY_ENTER])
        {
            if (sai || key[KEY_ESC])
            {
                exit(0);
            }

            else
            {
                menu ();
            }

        }

        desenhar_bordas_back ();
        gerar_snake ();
        desenhar_snake ();
        gerar_comida ();
        desenhar_comida ();
        desenhar_pontos ();
        update_tela ();

        while (!(sai || key[KEY_ESC]))
        {
            clear(buffer);

            update_direcao ();
            mover_snake ();
            if (gameover == 0)
            {
                gameover = 1;

                // VALORES INICIAIS
                tamanho = 3;
                direcao = direita;
                pontos = 0;
                velocidade = 100;

                break;
            }
            desenhar_bordas_back ();
            desenhar_snake ();
            desenhar_comida();
            desenhar_pontos();
            update_tela ();

            rest (velocidade);


        }
    }

    return 0;
}

END_OF_MAIN ();
END_OF_FUNCTION (sair);
