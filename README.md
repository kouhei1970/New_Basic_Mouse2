# New_Basic_Mouse2

福井大学のからくり工房Ｉ・ｓｙｓの方々に開発していただいた初心者用マウスのプログラムを少しづつ作っています。

##マイクロマウスのタイヤ直径の推定方法
マイクロマウスの前進距離はタイヤの直径に基づいて、タイヤを何回転させたら希望の前進距離になるかを計算して実現されます。
タイヤ直径はノギスなどで計測すれば概略の値が得られますが、ゴムタイヤの場合は、走行中につぶれて変形して、ノギスでの計測値とは実際に異なった値になっていると思われます。これは、ノギス計測値を使ってマイクロマウスを走行させた場合に、結果がずれますので容易にわかります。

そこで、概略の値を使って計算して、実際に走行させて、希望の走行距離との誤差を測定して、真の直径を推定します。推定式は以下のようになります。

Dt = D*(1+ e / L)

上式において

- Dt : タイヤ直径の推定値
- e : 走行時の誤差（設定走行距離からどのくらい行き過ぎたのか、足りなかったらマイナス
- L : 設定した走行距離（この距離を走ると考えて走行させた値）
