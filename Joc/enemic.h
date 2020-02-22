// jugador.h està inclòs al main.cpp

bool colisiona(float x1, float y1, float x2, float y2) {
    // suposam es dos són quadrats des mateix tamany
    if (abs(x1 - x2) < 0.7 && abs(y1 - y2) < 0.7) return true;
    return false;
}

class Enemic : public Jugador {
public:
    int color[3];

    DIRECCIO dir_anterior = STOP;

    void en_canviDir(DIRECCIO);
    void xoca(vector<Enemic*>&);

    Enemic() {};
    Enemic(int, Laberint*, int);
    Enemic(Jugador&);
    ~Enemic();

private:
    int novaposicio(Enemic*); // quina seria sa nova posició de s'enemic, per comprovar si xoquen
};

void Enemic::en_canviDir(DIRECCIO dir) {
    // això s'executarà quan sabem que la podem canviar
    if (dir == direccio) return;
    dir_anterior = direccio;
    direccio = dir;
}

int Enemic::novaposicio(Enemic* en) {
    switch (en->direccio) {
        case N: return en->x + (en->y - 1) * en->n; break;
        case E: return en->x + 1 + (en->y) * en->n; break;
        case S: return en->x + (en->y + 1) * en->n; break;
        case O: return en->x - 1+ (en->y) * en->n; break;
        case STOP: return en->x + (en->y) * en->n; break;
    }
}

/*void Enemic::xoca(vector<Enemic*>& enes) {
    int pos_actual = novaposicio(this);

    for (auto &x : enes) {
        int pos = novaposicio(x);
        if (pos == pos_actual) {
            direccio = (DIRECCIO)((direccio + 2) % 4);
            x->direccio = (DIRECCIO)((x->direccio + 2) % 4);
        }
    }
}*/

void Enemic::xoca(vector<Enemic*>& enes) {
    int w = radi * 2;
    for (auto &en : enes) {
        if (n * abs(x - en->x) <= w && m * abs(y - en->y) <= w) {
            // xoquen
            direccio = (DIRECCIO)((direccio + 2) % 4);
            en->direccio = (DIRECCIO)((en->direccio + 2) % 4);

            for (int i = 0; i < 50; i++) {
                moure(direccio, true);
                en->moure(en->direccio, true);
            }
        }
    }
}

Enemic::Enemic(int _radi, Laberint* _maze, int factor) {
    radi = _radi;
    maze = new Laberint(_maze->n, _maze->m);

    maze->n = _maze->n;
    maze->m = _maze->m;
    factor_vel = factor;

    n = maze->n;
    m = maze->m;
    
    maze->graella = _maze->graella;

    color[0] = random() % 150 + 100;
    color[1] = random() % 100 + 50;
    color[2] = random() % 150 + 100;

    factor_vel = random() % 200 + factor + 100;
    while ((int)factor_vel % n != 0) factor_vel++;
    vel = n / factor_vel;
    
    while (true) {
        int num = random() % (n * m);

        if (num % n > 6 && maze->graella[num] != MUR) {
            auto pos = D12(num, n);
            x = pos[0];
            y = pos[1];
            break;
        }
    }
}

Enemic::Enemic(Jugador& jug) {
    n = jug.n;
    m = jug.m;
    factor_vel = jug.factor_vel;
    maze = new Laberint(n, m);
    maze->graella = jug.maze->graella;
    radi = jug.radi;

    color[0] = random() % 150 + 100;
    color[1] = random() % 150 + 100;
    color[2] = random() % 150 + 100;

    factor_vel = random() % 200 + factor_vel + 100;
    while ((int)factor_vel % n != 0) factor_vel++;
    vel = n / factor_vel;

    // seleccionam posició aleatòria
    while (true) {
        int num = random() % n;

        if (num % n > 6 && maze->graella[num] != MUR) {
            auto pos = D12(num, n);
            x = pos[0];
            y = pos[1];
        }
    }
}
