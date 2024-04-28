typedef NewStoryGroupVectorFunction NewStoryGroupVectorFunctuon;

class NewStoryGroupVectorFunction
{
	static vector Division(vector v, float value)
	{
		if ( value == 0 ) {
			ErrorEx("Division by zero");
			return v;
		}

		v[0] = v[0] / value;
		v[1] = v[1] / value;
		v[2] = v[2] / value;

		return v;
	}

	static vector Multiplication(vector v, float value)
	{
		v[0] = v[0] * value;
		v[1] = v[1] * value;
		v[2] = v[2] * value;

		return v;
	}

	static float AngleBetweenVectors(vector a, vector b)
	{
		return Math.Acos( AngleBetweenVectorsCos( a, b ) );
	}

	static float AngleBetweenVectorsCos(vector a, vector b)
	{
		return vector.Dot( a, b ) / ( a.Length() * b.Length() );
	}

	static vector GroundPos(vector position)
	{
		position[1] = GetGame().SurfaceY(position[0], position[2]);
		return position;
	}

	static vector GroundOri(vector position)
	{
		return GetGame().GetSurfaceOrientation(position[0], position[2]);
	}

	static vector GroundPositionInRadius(vector position, float radius)
	{
		position[0] = Math.RandomFloat(position[0] - radius, position[0] + radius);
		position[2] = Math.RandomFloat(position[2] - radius, position[2] + radius);
		
		return GroundPos(position);
	}

	static vector PositionInRadius(vector position, float radius)
	{
		position[0] = Math.RandomFloat(position[0] - radius, position[0] + radius);
		position[2] = Math.RandomFloat(position[2] - radius, position[2] + radius);
		
		return position;
	}

	static vector PositionInArea(vector position, float radius1, float radius2)
	{
		return RandomPositionOnTheBorder(position, radius1 + Math.RandomFloat(radius1, radius2));
	}

	static vector RandomPositionOnTheBorder(vector center, float radius)
	{
		float rnd_angle = Math.RandomFloat(0, Math.PI2);

		center[0] = center[0] + radius * Math.Sin(rnd_angle);
		center[2] = center[2] + radius * Math.Cos(rnd_angle);

		return center;
	}

	static bool IsPointInSphere(vector c, float r, vector p)
	{
		return vector.Distance( p, c ) <= r;
	}

	static bool UndergroundPosition(out vector position)
	{
		float ground_height = GetGame().SurfaceY(position[0], position[2]);

		if (position[1] - ground_height < 0) {
			position[1] = ground_height;
			return true;
		}

		return false;
	}

	static bool InTheField(vector p, vector c, float r, out float d)
	{
		d = vector.Distance(p, c);
		return d < r;
	}
}
