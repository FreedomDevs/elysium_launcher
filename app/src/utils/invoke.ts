type InvokePayload = Record<string, unknown>;

export async function invoke<T = unknown>(
  cmd: string,
  args: InvokePayload = {}
): Promise<T> {
  const fn = (window as any)[cmd];
  if (typeof fn !== "function") {
    throw new Error(`Функция ${cmd} не найдена или не является функцией`);
  }

  const raw = await fn(JSON.stringify(args));

  if (typeof raw === "string") {
    try {
      return JSON.parse(raw) as T;
    } catch (e) {
      console.error("Ошибка при парсинге ответа функции:", raw);
      throw e;
    }
  } else {
    // Если raw уже объект — возвращаем сразу
    return raw as T;
  }
}
