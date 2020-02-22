//#include "laberint.h"

/*using namespace sf;
using namespace std;

float fact = 1;
bool fluix = true;

void render(Laberint* graella, RenderWindow* joc) {
    // suposam que és proporcional
    float ample = joc->getSize().x / (float)graella->n;
    
    RectangleShape rect = RectangleShape(Vector2f(ample, ample));

    // opcional:
    rect.setOutlineColor(Color(20, 20, 20));
    rect.setOutlineThickness(1);

    for (int i = 0; i < graella->m; i++) {
        for (int j = 0; j < graella->n; j++) {
            rect.setPosition(ample * j, ample * i);

            int pos = D21(j, i, graella->n);
            switch (graella->graella[pos]) {
                case VIST: case MONEDA: rect.setFillColor(Color(0, 0, 0, 0)); break;
                case NOVIST: rect.setFillColor(Color(130, 40, 40)); break;
                case MUR: rect.setFillColor(Color(140, 140, 140)); break;
            }
            if (graella->graella[pos] == LABEL::MONEDA) {
                float fase = ample * 0.25 * (0.75 + fact);
                CircleShape cercle = CircleShape(ample / 2 - fase);

                if (fluix) {
                    fact *= 0.998;
                    if (fact < 0.1) fluix = false;
                } else {
                    fact *= 1.002;
                    if (fact > 0.95) {
                        fluix = true;
                        fact = 1;
                    }
                }
                
                cercle.setFillColor(Color((1 - fact) * 255, (1 - fact) * 200, (1 - fact) * 10));
                cercle.setPosition(j * ample + fase, i * ample + fase);

                joc->draw(cercle);
            }
            joc->draw(rect);
        }
    }
}

void renderJug(Jugador* jug, RenderWindow* joc) {
    float ample = joc->getSize().x / (float)jug->maze->n;

    CircleShape cercle = CircleShape(jug->radi / 2);
    cercle.setFillColor(Color(30, 200, 30));

    cercle.setPosition(jug->x * ample, jug->y * ample);

    joc->draw(cercle);
}

void renderJug(Enemic* ene, RenderWindow* joc, Color color) {
    float ample = joc->getSize().x / (float)ene->maze->n;

    CircleShape cercle = CircleShape(ene->radi / 2);
    cercle.setFillColor(color);

    cercle.setPosition(ene->x * ample, ene->y * ample);

    joc->draw(cercle);
}

int joc(string ruta1, int _x, int W, int H, int bpp, int monedes, int maxvides, int v, int enemics) {
    auto s = VideoMode::getFullscreenModes();

    //int W = s[0].width, H = s[0].height, monedes = 10, recollides = 0, maxvides = 5;
    int temps = 0, recollides = 0;
    float ratio = W / (float)H, ferit = 0;
    int _y = _x / ratio - 1, vides = maxvides;
    
    bool perdut = false, pausa = false;
    
    srand(time(NULL));

    Font font = Font();
    font.loadFromFile(ruta1 + "SigmarOne-Regular.ttf");

    
    RenderWindow* joc = new RenderWindow(VideoMode(W, H, bpp), "Laberint", Style::Titlebar | Style::Close);
    //RenderWindow* joc = new RenderWindow(VideoMode(W, H), "Laberint", Style::Fullscreen);
    
    Laberint* maze = new Laberint(_x, _y);

    maze->generar();
    maze->monedes(monedes);

    Jugador* jug = new Jugador(joc->getSize().x / (float)maze->n, maze);
    vector<Enemic*> enes;

    for (int i = 0; i < enemics; i++) {
        enes.push_back(new Enemic(jug->radi, maze));
    }

    Texture textvida = Texture();
    textvida.loadFromFile(ruta1 + "vida.png");
    Sprite imgvida = Sprite(textvida);
    imgvida.scale(Vector2f(64 / (float)textvida.getSize().x, 64 / (float)textvida.getSize().x));
    
    while (joc->isOpen()) {
        bool pintar = false;
        Event e;
        while (joc->pollEvent(e)) {
            if (e.type == Event::Closed) joc->close();
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::Enter) {
                    // resetejam es joc
                    maze->esborrar();
                    maze->generar();
                    maze->monedes(monedes);
                    recollides = 0;
                    vides = maxvides;
                    pausa = false;
                    perdut = false;
                    jug = new Jugador(joc->getSize().x / (float)maze->n, maze);
                    for (int i = 0; i < enemics; i++) {
                        enes[i] = new Enemic(jug->radi, maze);
                    }
                }
                if (e.key.code == Keyboard::P) pausa = !pausa;
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::Right)) {
            jug->canviDIR(DIRECCIO::E);
        }
        if (Keyboard::isKeyPressed(Keyboard::Left)) {
            jug->canviDIR(DIRECCIO::O);
        }
        if (Keyboard::isKeyPressed(Keyboard::Up)) {
            jug->canviDIR(DIRECCIO::N);
        }
        if (Keyboard::isKeyPressed(Keyboard::Down)) {
            jug->canviDIR(DIRECCIO::S);
        }
        if (Keyboard::isKeyPressed(Keyboard::Space)) {
            jug->canviDIR(DIRECCIO::STOP);
        }
        
        if (pausa || perdut) goto RENDER;
        
        // movem es jugador
        jug->moure(jug->direccio);

        if (jug->ajustat()) {
            auto &casella_actual = maze->graella[jug->x + maze->n * jug->y];
            if (casella_actual == MONEDA) {
                recollides++;
                if (recollides == monedes) perdut = true; // realment hem gonyat per`així s'atura es joc
                if (recollides % 3 == 0) vides++;
                casella_actual = VIST;
            }
        }

        // comprovam si perdem vides
        if (temps > 0 && temps < 200) {
            temps++;
            temps %= 200;
        }

        for (auto en : enes) {
            if (colisiona(en->x, en->y, jug->x, jug->y)) {
                if (temps == 0) {
                    vides--;
                    temps = 1;
		            ferit = 180;
                    if (vides == 0) perdut = true;
                }
            }
        }

        // movem ets enemics
        for (auto &en : enes) {
            if (!en->ajustat()) continue;
            if (en->direccio != STOP && !en->colisiona(en->direccio)) {
                // miram si hi ha altres nexes per anar
                vector<DIRECCIO> dirs;
                for (int i = 0; i < 4; i++) {
                    auto dir = (DIRECCIO)i;
                    if (dir == O && en->x < 1 + en->vel) continue; // deixam sa primera columna lliure
                    if (dir % 2 != en->direccio % 2 && !en->colisiona(dir)) {
                        dirs.push_back(dir);
                    }

                }
                if (dirs.size() > 0) {
                    //for (int i = 0; i < 2; i++)
                    dirs.push_back(en->direccio);
                    
                    en->en_canviDir(dirs[random() % dirs.size()]); // en triam una aleatòria
                }
                continue;
            }

            // en cas que s'estampi contra sa paret
            vector<DIRECCIO> dirs;
            for (int i = 0; i < 4; i++) {
                auto dir = (DIRECCIO)i;
                if (dir == O && en->x < 1 + en->vel) continue; // deixam sa primera columna lliure
                if (en->direccio != dir && !en->colisiona(dir)) {
                    if (en->direccio % 2 == dir % 2) dirs.push_back(dir);
                    else {
                        for (int i = 0; i < 6; i++)
                            dirs.push_back(dir); // feim més probable si no és s'anterior
                    }
                }
            }
            if (dirs.size() > 0) en->en_canviDir(dirs[random() % dirs.size()]); // en triam una aleatòria
        }
    RENDER:
        // renderitzam
        joc->clear(Color(20,20,20));

        render(maze, joc);
        renderJug(jug, joc);

        for (auto &en : enes) {
            if (!pausa && !perdut) {
                if (en->ajustat() && en->x < 1 + en->vel && en->direccio == O) {
                    while (true) {
                        auto dir = (DIRECCIO)(random() % 3);
                        if (!en->colisiona(dir)) {
                            en->canviDIR(dir);
                            break;
                        }
                    }
                }
                en->moure(en->direccio, true);
            }
            renderJug(en, joc, Color(en->color[0], en->color[1], en->color[2]));
        }

        Text t = Text(to_string(recollides) + "/" + to_string(monedes), font);
        t.setPosition(W / 2 - 40, 10);
        t.setCharacterSize(60);
        t.setFillColor(Color(50, 200, 30));
        t.setOutlineColor(Color(20, 20, 20));
        t.setOutlineThickness(2);
        joc->draw(t);

        if (ferit > 1) {
            ferit -= 0.8;
            RectangleShape rect = RectangleShape(Vector2f(W, H));
            rect.setPosition(0, 0);
            rect.setFillColor(Color(230, 40, 40, (int)ferit));
            joc->draw(rect);
        }
        imgvida.setPosition(0, 0);

        for (int i = 0; i < vides; i++) {
            joc->draw(imgvida);
            imgvida.move(64, 0);
        }
        joc->display();
    }
    
    return 0;
}*/
