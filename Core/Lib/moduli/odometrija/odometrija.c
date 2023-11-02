/*
 * odometrija.c
 *
 *  Created on: Nov 2, 2023
 *      Author: Andrej Čilag
 */

#include "odometrija.h"
#include "../../periferije/tajmer/tajmer.h"

#include <stdint.h>
#include <math.h>

static float V = 0;
static float w = 0;
static float Vd = 0;
static float Vl = 0;
static float d = 0;
static float d_ot = 0;
static float inc2rad = 0;

static float fi = 0;
static float x = 0;
static float y = 0;

void
odometrija_init ()
{
  d_ot = 60; // Prečnik odometrijskog točka
  d = 250; // Rastojanje dva pasivna točka

  // 2048 - rezolucija enkodera
  // 4 - kvadraturno dekodiranje
  float N = (d * M_PI) / (d_ot * M_PI);
  float n = N * 2048 * 4;
  inc2rad = (2 * M_PI) / n;
}

// Računanje pozicije i orijentacije robota
void
odometrija_robot ()
{
  int16_t Vd_inc = tajmer_brzina_enkodera ();
  int16_t Vl_inc = Vd_inc;

  Vd = Vd_inc * inc2rad;
  Vl = Vl_inc * inc2rad;

  // Vd - Vl → desni koordinatni sistem
  w = (Vd - Vl) / d;
  V = (Vd + Vl) / 2;

  //fi = fi + w;
  fi += w;
  x += V*cos(fi);
  y += V*sin(fi);
}
