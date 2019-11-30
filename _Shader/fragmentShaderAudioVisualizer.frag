precision mediump float;

varying vec4 fColor;
varying vec2 fTexCoord;

uniform sampler2D sampler2d;

uniform float Factor1;

uniform float WINDOW_HEIGHT;
uniform float WINDOW_WIDTH;

uniform float LeftSpectrum[3];
uniform float RightSpectrum[3];

uniform int uBlurDirection;

void InterpolateValue(in vec3 iResolution, in float index, in float divisionValue, in bool left, out float value)
{
	float norm = 360.0/iResolution.x*index;
	float Floor = floor(norm)/divisionValue;
	float Ceil = ceil(norm)/divisionValue;
	
	if(left == true)
	{
		value = mix(LeftSpectrum[int(Floor)] * 0.8, LeftSpectrum[int(Ceil)] * 0.8, fract(norm));
	}
	else
	{
		value = mix(RightSpectrum[int(Floor)] * 0.8, RightSpectrum[int(Ceil)] * 0.8, fract(norm));
	}
}

vec4 SquareWaves(vec3 iResolution, float thickness, vec4 resultColor)
{
	//Square Waves Completed
	//float x = gl_FragCoord.x/ iResolution.x;
	//float y = gl_FragCoord.y / iResolution.y;
	float x = (sin(gl_FragCoord.x * 0.025) * 1000.0) / iResolution.x;
	float y = (sin(gl_FragCoord.y * 0.025) * 1000.0) / iResolution.y;
	
	float wave = 0.0;
	InterpolateValue(iResolution, (x * iResolution.x), 160.0, false, wave);
   
	wave = 0.5-wave/2.0; //centers wave
   
	float r = abs(thickness/((wave-y)));
	
	return resultColor +=
	vec4(
	
		r - abs( r * 0.2 * sin( Factor1 / 5.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 7.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 9.0)),
		
	1.0);
}

vec4 CircleMap(vec3 iResolution, float thickness, vec4 resultColor, float xCoord, float yCoord)
{
	float squareSineWaveX = sin(gl_FragCoord.x * 0.05 + Factor1 * 10.0) * 50.0; // - Modified from the circle
	float squareSineWaveY = sin(gl_FragCoord.y * 0.05 + Factor1 * 10.0) * 50.0; // - Modified from the circle
	
	float squareWaveX = (xCoord * 0.5) + squareSineWaveX;
	float squareWaveY = (yCoord * 0.5) + squareSineWaveY;
	
	float squareWaveMin = (yCoord * 0.5 - 10.0) + squareSineWaveX;
	float squareWaveMax = (yCoord * 0.5 + 10.0) + squareSineWaveX;
	
	float squareWaveDistance = squareWaveMax - squareWaveMin; // - This one to know the distance so your can choose the middle distance
	float squareWaveHalfDistance = squareWaveDistance/2.0; // - Half Distance
	float squareWaveMid = squareWaveMin + squareWaveHalfDistance; // Find your midpoint
	
	float x = (sin(gl_FragCoord.x * 0.025) * 500.0) / squareWaveX; //iResolution.x;
	float y = (sin(gl_FragCoord.y * 0.025) * 500.0) / squareWaveY; //iResolution.y;
	
	float wave = 0.0;
	InterpolateValue(iResolution, (x + squareWaveMid), 85.0, false, wave);
   
	wave = (0.5 - squareSineWaveX * 0.5) - wave / 1.0; //centers wave
   
	float r = abs(thickness/(wave-y));
	
	//sin(gl_FragCoord.x * 0.05 + Factor1 * 10.0) * 50.0
	
	return resultColor +=
	vec4(
	
		r - abs( r * 0.2 * sin( Factor1 / 5.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 7.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 9.0)),
		
	1.0);
}

vec4 StarMapV2(vec3 iResolution, float thickness, vec4 resultColor)
{
	float squareSineWaveX = sin(gl_FragCoord.x * 0.05 + Factor1 * 10.0) * 50.0; // - Modified from the circle
	float squareSineWaveY = sin(gl_FragCoord.y * 0.05 + Factor1 * 10.0) * 50.0; // - Modified from the circle
	
	float squareWaveX = (gl_FragCoord.x * 0.5) + squareSineWaveX;
	float squareWaveY = (gl_FragCoord.y * 0.5) + squareSineWaveY;
	
	float squareWaveMin = (gl_FragCoord.x * 0.5 - 10.0) + squareSineWaveX;
	float squareWaveMax = (gl_FragCoord.y * 0.5 + 10.0) + squareSineWaveX;
	
	float squareWaveDistance = squareWaveMax - squareWaveMin; // - This one to know the distance so your can choose the middle distance
	float squareWaveHalfDistance = squareWaveDistance/2.0; // - Half Distance
	float squareWaveMid = squareWaveMin + squareWaveHalfDistance; // Find your midpoint
	
	float x = (sin(gl_FragCoord.x * 0.025) * 500.0) / squareWaveX; //iResolution.x;
	float y = (sin(gl_FragCoord.y * 0.025) * 500.0) / squareWaveY; //iResolution.y;
	
	float wave = 0.0;
	InterpolateValue(iResolution, (x + squareWaveMid), 85.0, false, wave);
   
	wave = (0.5 - squareSineWaveX * 0.5) - wave / 1.0; //centers wave
   
	float r = abs(thickness/(wave-y));
	
	//sin(gl_FragCoord.x * 0.05 + Factor1 * 10.0) * 50.0
	
	return resultColor +=
	vec4(
	
		r - abs( r * 0.2 * sin( Factor1 / 5.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 7.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 9.0)),
		
	1.0);
}

vec4 StarMap(vec3 iResolution, float thickness, vec4 resultColor)
{
	float squareSineWaveX = sin(gl_FragCoord.x * 0.05 + Factor1 * 10.0) * 50.0; // - Modified from the circle
	float squareSineWaveY = sin(gl_FragCoord.y * 0.05 + Factor1 * 10.0) * 50.0; // - Modified from the circle
	
	float squareWaveX = (WINDOW_WIDTH * 0.5) + squareSineWaveX;
	float squareWaveY = (WINDOW_HEIGHT * 0.5) + squareSineWaveY;
	
	float squareWaveMin = (WINDOW_HEIGHT * 0.5 - 10.0) + squareSineWaveX;
	float squareWaveMax = (WINDOW_HEIGHT * 0.5 + 10.0) + squareSineWaveX;
	
	float squareWaveDistance = squareWaveMax - squareWaveMin; // - This one to know the distance so your can choose the middle distance
	float squareWaveHalfDistance = squareWaveDistance/2.0; // - Half Distance
	float squareWaveMid = squareWaveMin + squareWaveHalfDistance; // Find your midpoint
	
	float x = (sin(gl_FragCoord.x * 0.025) * 500.0) / squareWaveX; //iResolution.x;
	float y = (sin(gl_FragCoord.y * 0.025) * 500.0) / squareWaveY; //iResolution.y;
	
	float wave = 0.0;
	InterpolateValue(iResolution, (x + squareWaveMid), 85.0, false, wave);
   
	wave = (0.5 - squareSineWaveX * 0.5) - wave / 1.0; //centers wave
   
	float r = abs(thickness/(wave-y));
	
	//sin(gl_FragCoord.x * 0.05 + Factor1 * 10.0) * 50.0
	
	return resultColor +=
	vec4(
	
		r - abs( r * 0.2 * sin( Factor1 / 5.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 7.0 )),
		r - abs( r * 0.2 * sin( Factor1 / 9.0)),
		
	1.0);
}

vec4 SineWaveColored(vec4 color1, vec4 color2, float radiusSine,
float circleDistanceCheck, float maxCircleDistance, vec4 resultColor)
{
	//Sound Sine Wave - Color Gradient
	float left = LeftSpectrum[0];	
	
	float ySineWave = sin(gl_FragCoord.x * 0.05 + Factor1 * 10.0) * 50.0; // - Modified from the circle
	
	float yMin = (WINDOW_HEIGHT * 0.5 - radiusSine*2.0) + ySineWave/4.0 + (ySineWave * left * 1.0);// * 10.0; // - Your Minimum range
	float yMax = (WINDOW_HEIGHT * 0.5 + radiusSine*2.0) + ySineWave/4.0 + (ySineWave * left * 1.0);// * 10.0; // - Your Maximum range
	
	float yDistance = yMax - yMin; // - This one to know the distance so your can choose the middle distance
	float yHalfDistance = yDistance/2.0; // - Half Distance
	float yMid = yMin + yHalfDistance; // Find your midpoint
	
	//float minCircleDistanceSine = 150.0 + radiusSine * 0.1 * sin(0.03+4.0+LeftSpectrum[0]*5.0) * 1.0;
	
	if(gl_FragCoord.y < yMax && gl_FragCoord.y > yMin)
	{
		if(circleDistanceCheck > maxCircleDistance)
		{
			float offset = abs(gl_FragCoord.y - yMid) / (yHalfDistance * 0.95);
			vec4 sineColor = mix(color1, color2, offset); //color;
			return resultColor = sineColor;
		}
	}
	
	return resultColor;
}
 
vec4 SoundCircle(float x, float y, float radius, float minRadius,
float maxRadius, vec4 color1, vec4 color2, out float circleDistCheck,
out float maxCircleDist, vec4 texColor, vec4 resultColor)
{
	float circleDistanceCheck = distance(vec2(x, y), vec2(gl_FragCoord.x, gl_FragCoord.y));
	float minCircleDistance = minRadius + radius * sin(0.03+4.0) * 0.1;
	float maxCircleDistance = maxRadius + radius * sin(0.03+4.0+LeftSpectrum[0]*5.0) * 0.1;
	
	circleDistCheck = circleDistanceCheck;
	maxCircleDist = maxCircleDistance;
	
	float circleDistance =  maxCircleDistance - minCircleDistance; // - This one to know the distance so your can choose the middle distance
	float halfCircleDistance = circleDistance/2.0; // - Half Distance
	float midCircleDistance = minCircleDistance + halfCircleDistance; // Find your midpoint
	
	if(circleDistanceCheck > minCircleDistance && circleDistanceCheck < maxCircleDistance)
	{
		float offset = abs(circleDistanceCheck - midCircleDistance) / (halfCircleDistance * 0.95);
		return resultColor = mix(color1, color2, offset);
	}
	else
	{
		return resultColor += texColor;
	}
}

 
vec4 DottedSoundCircle(float x, float y, float radius, float minRadius,
float maxRadius, vec4 color1, vec4 color2, out float circleDistCheck,
out float maxCircleDist, vec4 texColor, vec4 resultColor)
{
	//float glX = gl_FragCoord.x + sin(gl_FragCoord.x * 0.01 + Factor1 * 10.0) * 50.0; // * 0.1 + 10.0;
	//float glY = gl_FragCoord.y + sin(gl_FragCoord.x * 0.01 + Factor1 * 10.0) * 50.0; // * 0.1 + 10.0;
	float glX = gl_FragCoord.x + sin(gl_FragCoord.x * LeftSpectrum[0]) * 50.0; // * 0.1 + 10.0;
	float glY = gl_FragCoord.y + sin(gl_FragCoord.y * RightSpectrum[0]) * 50.0; // * 0.1 + 10.0;
	
	float circleDistanceCheck = distance(vec2(x, y), vec2(glX, glY));
	//float circleDistanceCheck = distance(vec2(x, y), vec2(glX + radius * sin(0.03+4.0+LeftSpectrum[0]*5.0) * 0.1, glY + radius * sin(0.03+4.0+LeftSpectrum[0]*5.0) * 0.1));
	
	float minCircleDistance = minRadius + radius * sin(0.03+4.0) * 0.1;
	float maxCircleDistance = maxRadius + radius * sin(0.03+4.0) * 0.1; //* sin(0.03+4.0+LeftSpectrum[0]*5.0) * 0.1;
	
	circleDistCheck = circleDistanceCheck;
	maxCircleDist = maxCircleDistance;
	
	float circleDistance =  maxCircleDistance - minCircleDistance; // - This one to know the distance so your can choose the middle distance
	float halfCircleDistance = circleDistance/2.0; // - Half Distance
	float midCircleDistance = minCircleDistance + halfCircleDistance; // Find your midpoint
	
	if(circleDistanceCheck > minCircleDistance && circleDistanceCheck < maxCircleDistance)
	{
		float offset = abs(gl_FragCoord.y - minCircleDistance) / (midCircleDistance * 0.95);
		return resultColor = mix(color1, color2, offset); //color;
	}
	else
	{
		return resultColor += texColor;
	}
}

float gaussianFunction(float x)
{
	float variance = 0.15; //x should be 0-1.0 with this variance

	float alpha = -(x*x / (2.0*variance));
	return exp(alpha);
}


void GaussianBlur()
{
	float textureW = WINDOW_WIDTH;
	float textureH = WINDOW_HEIGHT;

	float radiusSize = 5.0;
	float totalWeight = 0.0;

	vec4 accumulatedColor;
	
	if(uBlurDirection == 0) //vertical blur
	{
		float u = fTexCoord.x;
		float y;
		
		for(y=-radiusSize; y<=radiusSize; y+=1.0)
		{
			float v = fTexCoord.y + y/textureH;
			
			if(v>=0.0 && v<=1.0)
			{
				float weight = gaussianFunction(y/radiusSize);
				accumulatedColor += texture2D(sampler2d, vec2(u,v)) * weight;
				totalWeight += 1.0;
			}
		}
		accumulatedColor /= (totalWeight * 0.39);
		gl_FragColor = accumulatedColor;
	}
	else if(uBlurDirection == 1) //horizontal blur
	{
		float v = fTexCoord.y;
		float x;
		
		for(x=-radiusSize; x<=radiusSize; x+=1.0)
		{
			float u = fTexCoord.x + x/textureW;
			
			if(u>=0.0 && u<=1.0)
			{
				float weight = gaussianFunction(x/radiusSize);
				accumulatedColor += texture2D(sampler2d, vec2(u,v)) * weight;
				totalWeight += 1.0;
			}
		}
		
		accumulatedColor /= (totalWeight * 0.39);
		gl_FragColor = accumulatedColor;
	}
}


void BlurCircle()
{
	float x = WINDOW_WIDTH * 0.5;
	float y = WINDOW_HEIGHT * 0.5;
	float glX = gl_FragCoord.x + sin(gl_FragCoord.x * LeftSpectrum[0]) * 50.0; // * 0.1 + 10.0;
	float glY = gl_FragCoord.y + sin(gl_FragCoord.y * RightSpectrum[0]) * 50.0; // * 0.1 + 10.0;
	float radius = 50.0;
	float minRadius = 150.0 * ((WINDOW_HEIGHT + WINDOW_WIDTH)/1400.0);
	float maxRadius = 155.0 * ((WINDOW_HEIGHT + WINDOW_WIDTH)/1400.0);
	
	float circleDistanceCheck = distance(vec2(x, y), vec2(glX, glY));
	float minCircleDistance = minRadius + radius;
	float maxCircleDistance = maxRadius + radius;
	
	float circleDistance =  maxCircleDistance - minCircleDistance;
	float halfCircleDistance = circleDistance/2.0;
	float midCircleDistance = minCircleDistance + halfCircleDistance;
	
	if(circleDistanceCheck > minCircleDistance)
	{
		GaussianBlur();
	}
}

void Normal()
{
	vec4 texColor  = texture2D(sampler2d, fTexCoord);
	texColor /= 2.0;
	vec4 combinedColor;
	combinedColor = fColor * texColor;
	
	vec4 resultColor;
	
	vec4 color1 = vec4(0.0, 1.0, 0.6, 1.0); // Color to change from
	vec4 color2 = vec4(0.47, 0.93, 0.87, 1.0); // Color to change to
	
	float thickness = 0.02;	
	vec3 iResolution = vec3(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_WIDTH * WINDOW_HEIGHT);
	
	float midCoordX = WINDOW_WIDTH * 0.5;
	float midCoordY = WINDOW_HEIGHT * 0.5;

	float radius = 50.0;
	float sinWaveRadius = 1.0 * ((WINDOW_HEIGHT + WINDOW_WIDTH)/1400.0);
	float minRadius = 150.0 * ((WINDOW_HEIGHT + WINDOW_WIDTH)/1400.0);
	float maxRadius = 155.0 * ((WINDOW_HEIGHT + WINDOW_WIDTH)/1400.0);
	float maxCircleDist;
	float circleDistCheck;
	
	resultColor = StarMap(iResolution, thickness, resultColor);
	//resultColor = CircleMap(iResolution, thickness, resultColor, WINDOW_WIDTH/800.0, WINDOW_HEIGHT/600.0);
	resultColor = SquareWaves(iResolution, thickness, resultColor);
	
	resultColor = SoundCircle(midCoordX, midCoordY, radius, minRadius/2.0, maxRadius/2.0, color1, color2, circleDistCheck, maxCircleDist, texColor, resultColor);
	resultColor = DottedSoundCircle(midCoordX, midCoordY, radius, minRadius, maxRadius, color1, color2, circleDistCheck, maxCircleDist, texColor, resultColor);
	
	resultColor = SineWaveColored(color1, color2, sinWaveRadius, circleDistCheck, maxCircleDist, resultColor);
	
	gl_FragColor = resultColor;
}

void main()
{
	Normal();
	BlurCircle();
}