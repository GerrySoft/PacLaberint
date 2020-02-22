enum DIRECCIO {N = 0, E = 1, S = 2, O = 3, STOP = 4};

class Jugador {
public:
    float x = 0, y = 0, factor_vel = 1700;
    double vel = 0;
    int n, m, radi, temporitzador = 0;
    Laberint* maze;
    DIRECCIO direccio = STOP;

    void moure(DIRECCIO, bool);
    void canviDIR(DIRECCIO);
    bool colisiona(DIRECCIO); // comprova si col·lisiona al girar
    bool ajustat(); // comprova si no està entre dues caselles

    Jugador() {};
    Jugador(int, Laberint*, int factor);
    ~Jugador() {};
};

Jugador::Jugador(int _radi, Laberint* lab, int factor) {
    n = lab->n;
    m = lab->m;
    x =  0;
    y = m / 2;
    factor_vel = factor;

    while ((int)factor_vel % n != 0) factor_vel++;

    vel = n / factor_vel;
    radi = _radi;
    
    maze = new Laberint(lab->n, lab->m);
    maze->graella = lab->graella;

    while (maze->graella[n * y] == LABEL::MUR) y++;
}

bool Jugador::colisiona(DIRECCIO dir) {
    // ja suposam que x, y són sencers
    switch (dir) {
        case N:
            if (y < 1) return true; // aquest és pes límits
            else if (maze->graella[y * n + x - n] == LABEL::MUR) return true; // aquest per si hi ha un mur
            return false;
            break;
        case E:
            if (x + 1 >= n) return true; // aquest és pes límits
            else if (maze->graella[y * n + x + 1] == LABEL::MUR) return true; // aquest per si hi ha un mur
            return false;
            break;
        case S:
            if (y + 1 >= m) return true; // aquest és pes límits
            else if (maze->graella[y * n + x + n] == LABEL::MUR) return true; // aquest per si hi ha un mur
            return false;
            break;
        case O:
            if (x < 1) return true; // aquest és pes límits
            else if (maze->graella[y * n + x - 1] == LABEL::MUR) return true; // aquest per si hi ha un mur
            return false;
            break;
    }
    return false;
}

bool Jugador::ajustat() {
    if (temporitzador == 0) return true;
    return false;
}

void Jugador::canviDIR(DIRECCIO dir) {
    if (ajustat() && !colisiona(dir)) {
        direccio = dir;
    }
}

void Jugador::moure(DIRECCIO dir, bool enemic = false) {
    if (direccio == STOP) return;

    if (++temporitzador > factor_vel / n) temporitzador = 0;
    switch (dir) {
        case N:
            if (y > 0) y -= vel;
            if (temporitzador == 0) {
                x = (int)x;
                y = (y < 0.5) ? 0 : ceil(y);
            }
            break;
        case E:
            if (x < n - 1) x += vel;
            if (temporitzador == 0) {
                x = (int)x;
                y = (int)y;
            }
            break;
        case S:
            if (y < m - 1) y += vel;
            if (temporitzador == 0) {
                x = (int)x;
                y = (int)(y);
            }
            break;
        case O:
            if (x > 0) x -= vel;
            if (temporitzador == 0) {
                x = (x < 0.5) ? 0 : ceil(x);
                y = (int)y;
            }
            break;
    }

    if (!enemic && ajustat() && colisiona(dir)) {
        direccio = STOP;
    }
}
