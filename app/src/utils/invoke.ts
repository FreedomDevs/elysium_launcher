type InvokePayload = Record<string, unknown>;

export async function invoke<T = unknown>(
  cmd: string,
  args: InvokePayload = {}
): Promise<T> {
  if (typeof (window as any).cppFunction !== "function") {
    throw new Error("C++ function is not available");
  }

  const payload = {
    cmd,
    ...args,
  };

  const raw = await (window as any).cppFunction(JSON.stringify(payload));

  try {
    return JSON.parse(raw) as T;
  } catch (e) {
    console.error("Ошибка при парсинге ответа C++:", raw);
    throw e;
  }
}
