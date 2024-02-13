//
// Created by vm on 24.30.1.
//
#pragma once

class Renderer;

class RendererConfig {
private:
    Renderer& renderer;

public:
    void blink(bool doBlink);

    void blink();

    explicit RendererConfig(Renderer &renderer) : renderer(renderer) {};
};

