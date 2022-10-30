#include "window.hpp"
#include <iostream>
void Window::onEvent(SDL_Event const &event) {
  // Keyboard events
  
  if (event.type == SDL_KEYDOWN) {
    if ((event.key.keysym.sym == SDLK_UP) && m_ship.m_translation.y<1.0f)
      m_ship.m_translation.y +=0.01f;
    if ((event.key.keysym.sym == SDLK_DOWN) && m_ship.m_translation.y>-1.0f)
      m_ship.m_translation.y -=0.01f;
    
    if ((event.key.keysym.sym == SDLK_w) && m_nave.m_translationO.y<1.0f)
      m_nave.m_translationO.y +=0.01f;
    if ((event.key.keysym.sym == SDLK_s) && m_nave.m_translationO.y>-1.0f)
      m_nave.m_translationO.y -=0.01f;
  }

}

void Window::onCreate() {
  auto const assetsPath{abcg::Application::getAssetsPath()};

  // Load a new font
  auto const filename{assetsPath + "Inconsolata-Medium.ttf"};
  m_font = ImGui::GetIO().Fonts->AddFontFromFileTTF(filename.c_str(), 60.0f);
  if (m_font == nullptr) {
    throw abcg::RuntimeError("Cannot load font file");
  }

  // Create program to render the other objects
  m_objectsProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "objects.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "objects.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  // Create program to render the stars
  m_starsProgram =
      abcg::createOpenGLProgram({{.source = assetsPath + "stars.vert",
                                  .stage = abcg::ShaderStage::Vertex},
                                 {.source = assetsPath + "stars.frag",
                                  .stage = abcg::ShaderStage::Fragment}});

  abcg::glClearColor(0, 0, 0, 1);

#if !defined(__EMSCRIPTEN__)
  abcg::glEnable(GL_PROGRAM_POINT_SIZE);
#endif

  // Start pseudo-random number generator
  m_randomEngine.seed(
      std::chrono::steady_clock::now().time_since_epoch().count());

  restart();
}

void Window::restart() {
  m_gameData.m_state = State::Playing;

  m_starLayers.create(m_starsProgram, 25);
  m_ship.create(m_objectsProgram);
  m_nave.create(m_objectsProgram);
  m_balls.create(m_objectsProgram, 1);

}

void Window::onUpdate() {
  auto const deltaTime{gsl::narrow_cast<float>(getDeltaTime())};

  // Wait 5 seconds before restarting
  if (m_gameData.m_state != State::Playing &&
      m_restartWaitTimer.elapsed() > 5) {
    restart();
    return;
  }
  for (auto &ball : m_balls.m_balls) {
     if(ball.m_translation.x<=-1.0f){
  	count_X++;
  	restart();
  	return;
     } else if(ball.m_translation.x>=+1.0f){
  	count_O++;
  	restart();
  	return;
     }
  }
  

  m_ship.update(m_gameData, deltaTime);
  m_nave.update(m_gameData, deltaTime);
  m_starLayers.update(m_ship, deltaTime);
  m_balls.update(m_ship, deltaTime);


  if (m_gameData.m_state == State::Playing) {
    checkCollisions();
    checkWinCondition();
  }
}

void Window::onPaint() {
  abcg::glClear(GL_COLOR_BUFFER_BIT);
  abcg::glViewport(0, 0, m_viewportSize.x, m_viewportSize.y);

  m_starLayers.paint();
  m_balls.paint();

  m_ship.paint(m_gameData,1);
  m_nave.paint(m_gameData,0);
}

void Window::onPaintUI() {
  abcg::OpenGLWindow::onPaintUI();

  {
    auto const size{ImVec2(300, 150)};
    auto const positionX{ImVec2((m_viewportSize.x- 2*size.x) / 2.0f,
                               (m_viewportSize.y - size.y))};
                               
    auto const positionO{ImVec2((m_viewportSize.x + size.x) / 2.0f,
                               (m_viewportSize.y - size.y))};
    ImGui::SetNextWindowPos(positionX);
    ImGui::SetNextWindowSize(size);
    ImGuiWindowFlags const flagsX{ImGuiWindowFlags_NoBackground |
                                 ImGuiWindowFlags_NoTitleBar |
                                 ImGuiWindowFlags_NoInputs};
    ImGui::Begin(" ", nullptr, flagsX);
    ImGui::PushFont(m_font);
    ImGui::Text("Left: %i",count_O);
    ImGui::Text("Right: %i",count_X);
    ImGui::PopFont();
    ImGui::End();
  }
}

void Window::onResize(glm::ivec2 const &size) {
  m_viewportSize = size;

  abcg::glClear(GL_COLOR_BUFFER_BIT);
}

void Window::onDestroy() {
  abcg::glDeleteProgram(m_starsProgram);
  abcg::glDeleteProgram(m_objectsProgram);

  m_balls.destroy();

  m_ship.destroy();
  m_nave.destroy();
  m_starLayers.destroy();
}

void Window::checkCollisions() {
  
  for (auto &ball : m_balls.m_balls) {
    auto const ballTranslation{ball.m_translation};
    auto const distance_X{
        glm::distance(m_ship.m_translation, ballTranslation)};
    auto const distance_O{
        glm::distance(m_nave.m_translationO, ballTranslation)};
    auto const m_ship_up{m_ship.m_translation.y + m_ship.m_scale};
    auto const m_ship_down{m_ship.m_translation.y - m_ship.m_scale};
    
    auto const m_nave_up{m_nave.m_translation.y + m_ship.m_scale};
    auto const m_nave_down{m_nave.m_translation.y - m_ship.m_scale};
    

    if((distance_X < m_ship.m_scale * 0.9f && ball.m_translation.x>=0.95f)||(distance_O < m_nave.m_scale * 0.9f && ball.m_translation.x<=-0.95f)){
    ball.m_velocity.x =  -1*(ball.m_velocity.x);
    }
    }

}

void Window::checkWinCondition() {
  /*if (m_asteroids.m_asteroids.empty()) {
    m_gameData.m_state = State::Win;
    m_restartWaitTimer.restart();
  }*/
}
