import java.text.DecimalFormat;
import java.util.Vector;

class Point {

	public double xCoord; // X
	public double yCoord; // Y

	public Point(double xCoord, double yCoord) {
		this.xCoord = xCoord;
		this.yCoord = yCoord;
	}
}

public class CalculateFunctionValues {

	// Вычисляет значение функции f(x) для заданного x
	public static double calculateF(double currentX) {
		if (currentX >= -8.0 && currentX <= -5.0) {
			return -3.0;
		} else if (currentX > -5.0 && currentX <= -3.0) {
			return (3.0 / 2.0) * currentX + 9.0 / 2.0;
		} else if (currentX > -3.0 && currentX <= 3.0) {
			double argSqrt = 1.0 - Math.pow(currentX / 3.0, 2);
			if (argSqrt < 0) {
				argSqrt = 0; // Prevent NaN for small negative values
			}
			return 3.0 * Math.sqrt(argSqrt);
		} else if (currentX > 3.0 && currentX <= 8.0) {
			return (3.0 / 5.0) * currentX - 9.0 / 5.0;
		} else if (currentX > 8.0 && currentX <= 10.0) {
			return 3.0;
		}
		return 0.0; // Default value for X outside the given range
	}

	// Вычисляет значения функции в заданном диапазоне и шагом, заполняя вектор
	// точек
	public static void calculateFunction(
		double startX,
		double endX,
		double stepX,
		Vector<Point> pointsVec
	) {
		for (
			double currentX = startX;
			currentX <= endX + stepX / 2.0;
			currentX += stepX
		) {
			pointsVec.add(new Point(currentX, calculateF(currentX)));
		}
	}

	// Выводит данные из вектора точек в виде отформатированной таблицы
	public static void printTable(final Vector<Point> pointsVec) {
		System.out.println(new String(new char[20]).replace('\0', '-'));
		System.out.printf("%-10s%-10s%n", "X", "F(X)");
		System.out.println(new String(new char[20]).replace('\0', '-'));

		DecimalFormat df = new DecimalFormat("0.0000");
		for (final Point point : pointsVec) {
			System.out.printf(
				"%-10s%-10s%n",
				df.format(point.xCoord),
				df.format(point.yCoord)
			);
		}
		System.out.println(new String(new char[20]).replace('\0', '-'));
	}

	public static void calculateFunctionValues() {
		// Начальное и конечное значения X
		final double xStart = -8.0;
		final double xEnd = 10.0;
		// Шаг изменения X
		final double dxVal = 0.1;
		// Вектор для хранения пар значений X и F(X)
		Vector<Point> graphPoints = new Vector<>();

		// Расчет значений функции и заполнение вектора
		calculateFunction(xStart, xEnd, dxVal, graphPoints);
		// Вывод таблицы с отформатированными данными
		printTable(graphPoints);
	}
}
