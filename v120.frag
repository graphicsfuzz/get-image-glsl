#version 120

void main(void) {
  gl_FragColor = vec4(gl_FragCoord.x/256.0, gl_FragCoord.y/256.0, 1.0, 1.0);
}
