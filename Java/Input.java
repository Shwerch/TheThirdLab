// Content of src/input.java
import java.util.InputMismatchException;
import java.util.Scanner;
import java.util.Vector;

public class Input {

	private static Scanner scanner = new Scanner(System.in);

	public static void clearInput() {
		if (scanner.hasNextLine()) {
			scanner.nextLine(); // Consume the rest of the invalid line
		}
	}

	public static <T> T getValue(String message, Class<T> type) {
		T value = null;
		while (true) {
			System.out.print(message + ": ");
			try {
				if (type == Integer.class) {
					value = type.cast(scanner.nextInt());
				} else if (type == Double.class) {
					value = type.cast(scanner.nextDouble());
				} else if (type == Long.class) {
					value = type.cast(scanner.nextLong());
				} else if (type == String.class) {
					value = type.cast(scanner.next()); // For single word
				}
				scanner.nextLine(); // Consume the rest of the line
				break;
			} catch (InputMismatchException e) {
				System.out.println(
					"Invalid input. Please enter a valid " +
					type.getSimpleName() +
					"."
				);
				clearInput(); // Clear the invalid input from the scanner
			}
		}
		return value;
	}

	public static String getLine(String message) {
		System.out.print(message + ": ");
		String input = scanner.nextLine();
		System.out.println();
		return input;
	}

	public static <T extends Number> Vector<T> getVector(
		String message,
		Class<T> type
	) {
		Vector<T> result = new Vector<>();
		String line;

		while (true) {
			clearInput();
			System.out.print(message + ": ");
			line = scanner.nextLine();
			Scanner lineScanner = new Scanner(line);
			result.clear();
			boolean error = false;
			while (lineScanner.hasNext()) {
				try {
					if (type == Integer.class) {
						result.add(type.cast(lineScanner.nextInt()));
					} else if (type == Double.class) {
						result.add(type.cast(lineScanner.nextDouble()));
					} else if (type == Long.class) {
						result.add(type.cast(lineScanner.nextLong()));
					} else {
						System.out.println(
							"Unsupported type for vector input."
						);
						error = true;
						break;
					}
				} catch (InputMismatchException e) {
					System.out.println(
						"Invalid number in sequence. Please enter integers separated by spaces."
					);
					error = true;
					break;
				}
			}
			lineScanner.close();
			if (!error && !result.isEmpty()) {
				break;
			} else if (result.isEmpty() && !error) {
				System.out.println(
					"Input cannot be empty. Please enter numbers separated by spaces."
				);
			}
		}
		return result;
	}
}
