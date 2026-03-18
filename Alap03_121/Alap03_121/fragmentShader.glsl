#version 430
out vec4 color;
uniform vec2 u_circlePos;      //dinamikus körpozíció
uniform float u_lineYOffset;   //szakasz eltolása
uniform bool u_isIntersecting; //metszés állapota

void main(void){
    vec2 st=gl_FragCoord.xy;
    vec3 finalColor=vec3(1.0, 1.0, 0.7); 

    //kör rajzolása 
    float dist=distance(st, u_circlePos);
    float r=50.0;

    if (dist<=r){
        vec3 centerColor=u_isIntersecting ? vec3(1.0, 0.0, 0.0): vec3(0.0, 1.0, 0.0);
        vec3 borderColor=u_isIntersecting ? vec3(0.0, 1.0, 0.0): vec3(1.0, 0.0, 0.0);
        
        float t=dist/r;
        finalColor=mix(centerColor, borderColor, t);
    }

    //mozgatható szakasz
    float lineY=300.0+u_lineYOffset;
    if (st.x >= 200.0 && st.x<=400.0 && abs(st.y-lineY)<= 1.5){
        finalColor=vec3(0.6, 0.8, 1.0); 
    }

    color=vec4(finalColor, 1.0);
}