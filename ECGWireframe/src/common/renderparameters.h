#ifndef RENDERPARAMETERS_H
#define RENDERPARAMETERS_H

class RenderParameters {

public:

	RenderParameters();

	explicit RenderParameters(
			int N,
			int M1,
			int M2
	);

	void setN(int N);
	void setM1(int M1);
	void setM2(int M2);

	int getN() const;
	int getM1() const;
	int getM2() const;

private:

	int m_N;
	int m_M1;
	int m_M2;
};

#endif // RENDERPARAMETERS_H
