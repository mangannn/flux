class Player: public Object {

public:

  RectangleShape rect;
  Texture tRunning;
  Texture tStanding;
  //Texture *currentTexture;
  Sprite sBody;
  float ani, previousAni;

  Player(Vector2f posParam, Color colorParam):
    Object(posParam, Vector2f(0,-.1), 10, 5),
    rect(Vector2f(10, 10))
  {
    rect.setFillColor(colorParam);
    tRunning.loadFromFile("images/player_running.png");
    tRunning.setSmooth(true);
    tStanding.loadFromFile("images/player_standing.png");
    tStanding.setSmooth(true);
    //currentTexture = &tRunning;
    sBody.setTexture(tStanding);
    sBody.setOrigin(100,150);
    sBody.setScale(0.4, 0.4);
    sBody.setRotation(0);
    ani = 0;
    previousAni = ani;
    sBody.setTextureRect(Rect<int>(200 * floor(ani), 0, 200, 300));
    sBody.setPosition(pos);
  }

    virtual ~Player() {}

  virtual void draw() {
    Vector2f v(rect.getSize().x / 2.0, rect.getSize().y / 2.0);
    rect.setPosition(pos + (rect.getSize() / 2.0f));
    //window->draw(rect);

    //sBody.setTexture(*currentTexture);
    if (size(vel) * .06 < .05) {
      ani += .05;
      sBody.setTexture(tStanding);
    }
    else {
      ani += size(vel) * .06;
      sBody.setTexture(tRunning);
    }
    if (ani*200 >= sBody.getTexture()->getSize().x)
      ani -= sBody.getTexture()->getSize().x / 200;
    if (floor(ani) != previousAni) {
      sBody.setTextureRect(Rect<int>(200 * floor(ani), 0, 200, 300));
      sBody.setPosition(pos);
      if (angle(vel) != -1) sBody.setRotation(0 + angle(vel));
      previousAni = floor(ani);
    }

    window->draw(sBody);
  }

};
