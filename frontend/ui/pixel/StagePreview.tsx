import { FIELD_SIZE } from '../../../src/engine/constants'
import type { RawStageConfig } from '../../../src/engine/types'
import { renderStagePreview } from '../../../src/render/stagePreview'

interface Props {
  stage: RawStageConfig | null
  x?: number
  y?: number
  scale?: number
}

/** 关卡缩略预览；stage 为空时渲染纯黑占位 */
export default function StagePreview({ stage, x = 0, y = 0, scale = 1 }: Props) {
  return (
    <g transform={`translate(${x}, ${y}) scale(${scale})`}>
      {stage == null ? (
        <rect width={FIELD_SIZE} height={FIELD_SIZE} fill="#000000" />
      ) : (
        <image href={renderStagePreview(stage)} width={FIELD_SIZE} height={FIELD_SIZE} />
      )}
    </g>
  )
}
